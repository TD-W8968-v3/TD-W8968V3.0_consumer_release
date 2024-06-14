/*  Copyright(c) 2009-2012 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 * file		xt_massurl.c
 * brief	match host for GET/POST/CONNECT request
 * details	
 *
 * author		Yang Lei
 * version	
 * date		06Mar12
 *
 * history 	\arg	
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/list.h>
#include <linux/textsearch.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_massurl.h>

MODULE_AUTHOR("Yang Lei");
MODULE_DESCRIPTION("Xtables: match host for GET/POST/CONNECT request");
MODULE_LICENSE("GPL");

#ifdef XT_MASSURL_DEBUG
#define DPRINT(args...)		printk(args)
#else
#define DPRINT(args...)
#endif

#define SEARCH_ALGO		"kmp"
#define METHOD_LENGTH	10
#define MAX_URL_LENGTH	128

static struct xt_massurl_ts {
	struct list_head list;
	unsigned short index;
	struct ts_config *ts;
} glbTsAddr[XT_MASSURL_MAX_ENTRY];

static struct list_head glbTsHead;

static DEFINE_RWLOCK(massurl_lock);

enum {
	PATTERN_HOST,
	PATTERN_LINE,
};

static struct {
	const char *pattern;
	size_t len;
	struct ts_config *ts;
} glbTs[] = {
	[PATTERN_LINE]	= {"\n", 1},
	[PATTERN_HOST]	= {"\nHost:", 6},
};

enum {
	GET_METHOD,
	POST_METHOD,
	HEAD_METHOD,
};

static const struct {
	const char *method;
	size_t len;
} glbMethods[] = {
	[GET_METHOD]		= {"GET ", 4},
	[POST_METHOD]		= {"POST ", 5},
	[HEAD_METHOD]		= {"HEAD ", 8},
};

static void free_entry(struct xt_massurl_ts *conf)
{
	if (conf->ts)
	{
		list_del(&conf->list);
		textsearch_destroy(conf->ts);
		conf->ts = NULL;
	}
}

static int set_entry(void __user *user, unsigned int len)
{
	struct xt_massurl_url_info info;
	struct xt_massurl_ts *conf;
	struct ts_config *ts;

	if (copy_from_user(&info, user, sizeof(info))) /* Get info */
	{
		return -EFAULT;
	}
	DPRINT("xt_massurl set: %d %s\n", info.index, info.addr);
	if (info.index >= XT_MASSURL_MAX_ENTRY)
	{
		printk(KERN_WARNING "url index overflow\n");
		return -EINVAL;
	}
	conf = &glbTsAddr[info.index];

	if (info.addr[0] == '\0') /* delete */
	{
		write_lock_bh(&massurl_lock);
		free_entry(conf);
		goto ret;
	}

	ts = textsearch_prepare(SEARCH_ALGO, info.addr, strlen(info.addr),
							GFP_KERNEL, TS_AUTOLOAD|TS_IGNORECASE);
	if (IS_ERR(ts))
	{
		return -EFAULT;
	}
	
	write_lock_bh(&massurl_lock);
	free_entry(conf);
	conf->ts = ts;
	list_add(&conf->list, &glbTsHead);

ret:
	write_unlock_bh(&massurl_lock);
	return 0;
}

static bool match_addr(const struct xt_massurl_info *info, const char *text)
{
	struct ts_state ts;
	const struct xt_massurl_ts *conf;
	int len = strlen(text);
	bool ret = false;

	read_lock(&massurl_lock);
	list_for_each_entry(conf, &glbTsHead, list)
	{
		if (XT_MASSURL_BIT_ISSET(info->indexBits, conf->index))
		{
			if (textsearch_find_continuous(conf->ts, &ts, text, len) != UINT_MAX)
			{
				DPRINT("xt_massurl match: %d %s\n", conf->index, text);
				ret = true;
				break;
			}
		}
	}
	read_unlock(&massurl_lock);

	return ret;
}

static bool match_dns(const struct sk_buff *skb, const struct xt_match_param *par,
					  const struct xt_massurl_info *info)
{
	char buf[MAX_URL_LENGTH];
	const char *ptr;
	unsigned offset = par->thoff + sizeof(struct udphdr) + 12;
	unsigned len, i;

	if (offset + 7 > skb->len) /* \x01A\x00\x00\x01\x00\x01 */
	{
		return false;
	}
	len = min_t(unsigned, MAX_URL_LENGTH - 1, skb->len - offset);

	ptr = skb_header_pointer(skb, offset, len, buf);
	if (ptr != buf)
	{
		if (unlikely(ptr == NULL))
		{
			return false;
		}
		memcpy(buf, ptr, len);
	}
	buf[len] = '\0';

	for (i = 0; i < len && (offset = buf[i]) != 0; i += offset + 1)
	{
		buf[i] = '.';
	}
	ptr = buf + 1;

	DPRINT("xt_massurl dns:%s\n", ptr);

	/* find pattern in ptr */
	return match_addr(info, ptr);
}

static bool is_http_packet(const struct sk_buff *skb, unsigned offset)
{
	char buf[METHOD_LENGTH];
	const char *ptr;
	int i;

	if (offset + METHOD_LENGTH < skb->len)
	{
		/* check method */
		ptr = skb_header_pointer(skb, offset, METHOD_LENGTH, buf);
		if (likely(ptr != NULL))
		{
			for (i = 0; i < ARRAY_SIZE(glbMethods); i++)
			{
				if (strncasecmp(ptr, glbMethods[i].method, glbMethods[i].len) == 0)
				{
					return true;
				}
			}
		}
	}

	/* no valid method, maybe not http packet */
	return false;
}

static bool match_host(struct sk_buff *skb, const struct xt_massurl_info *info, unsigned offset)
{
	struct ts_state ts;
	char host[MAX_URL_LENGTH];
	char *ptr = host;
	unsigned len;

	/* find host part */
	len = skb_find_text(skb, offset, skb->len, glbTs[PATTERN_HOST].ts, &ts);
	if (len == UINT_MAX)
	{
		return false;
	}
	offset += len + glbTs[PATTERN_HOST].len;
	len = skb_find_text(skb, offset, skb->len, glbTs[PATTERN_LINE].ts, &ts);
	if (len == UINT_MAX)
	{
		return false;
	}
	len = min_t(unsigned, XT_MASSURL_MAX_LENGTH - 1, len);
	if (skb_copy_bits(skb, offset, host, len)) /* error */
	{
		return false;
	}
	if (likely(host[len - 1] == '\r'))
	{
		len--;
	}
	host[len] = '\0';
	if (likely(host[0] == ' '))
	{
		ptr++;
	}

	DPRINT("xt_massurl http:%s\n", ptr);

	/* find pattern in ptr */
	return match_addr(info, ptr);
}

static bool match(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_massurl_info *info = par->matchinfo;
	struct tcphdr _th;
	const struct tcphdr *th;
	unsigned offset = par->thoff;

	if (info->type == XT_MASSURL_MATCH_DNS)
	{
		return match_dns(skb, par, info);
	}

	/* Get TCP header */
	th = skb_header_pointer(skb, offset, sizeof(_th), &_th);
	if (unlikely(th == NULL))
	{
		return false;
	}
	offset += th->doff * 4;

	if (info->type == XT_MASSURL_MATCH_HTTP)
	{
		return is_http_packet(skb, offset);
	}
	else if (info->type == XT_MASSURL_MATCH_NOTHTTP)
	{
		return !is_http_packet(skb, offset);
	}

	/* search pattern in http packet */
	return match_host((struct sk_buff *)skb, info, offset);
}

static struct xt_match massurl_mt_reg[] __read_mostly = {
	{
	.family			= NFPROTO_IPV4,
	.name			= "massurl",
	.match			= match,
	.matchsize		= sizeof(struct xt_massurl_info),
	.me				= THIS_MODULE,
	.data			= (unsigned long)set_entry,
	},
	{
	.family			= NFPROTO_IPV6,
	.name			= "massurl",
	.match			= match,
	.matchsize		= sizeof(struct xt_massurl_info),
	.me				= THIS_MODULE,
	},
};

static int __init mt_init(void)
{
	int i;
	int ret;

	/* init addr list */
	INIT_LIST_HEAD(&glbTsHead);
	for (i = 0; i < XT_MASSURL_MAX_ENTRY; i++)
	{
		glbTsAddr[i].index = i;
		glbTsAddr[i].ts = NULL;
	}

	/* init ts_config */
	for (i = 0; i < ARRAY_SIZE(glbTs); i++)
	{
		glbTs[i].ts = textsearch_prepare(SEARCH_ALGO, glbTs[i].pattern, glbTs[i].len,
										 GFP_KERNEL, TS_AUTOLOAD|TS_IGNORECASE);
		if (IS_ERR(glbTs[i].ts))
		{
			ret = PTR_ERR(glbTs[i].ts);
			while (--i >= 0)
			{
				textsearch_destroy(glbTs[i].ts);
			}
			return ret;
		}
	}

	/* register match */
	return xt_register_matches(massurl_mt_reg, ARRAY_SIZE(massurl_mt_reg));
}

static void __exit mt_exit(void)
{
	struct xt_massurl_ts *conf, *tmp;
	int i = ARRAY_SIZE(glbTs);

	list_for_each_entry_safe(conf, tmp, &glbTsHead, list)
	{
		free_entry(conf);
	}

	xt_unregister_matches(massurl_mt_reg, ARRAY_SIZE(massurl_mt_reg));

	while (--i >= 0)
	{
		textsearch_destroy(glbTs[i].ts);
	}
}

module_init(mt_init);
module_exit(mt_exit);
