/*  Copyright(c) 2009-2012 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 * file		libxt_massurl.c
 * brief	match host or url for GET/POST/CONNECT request
 * details	
 *
 * author		Yang Lei
 * version	
 * date		07Mar12
 *
 * history 	\arg	
 */

/*example:
 * iptables -A FORWARD -p tcp -m massurl --type host --index 1 -j DROP
 */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <xtables.h>
#include <linux/netfilter/xt_massurl.h>

#define ARG_TYPE	0x01
#define ARG_INDEX	0x02

/* Function which prints out usage message. */
static void help(void)
{
	printf(
    "MASSURL v%s set url match index in rules. Options:\n"
    "  --type	match type [host/url/dns/http]\n"
    "  --index	with hex-value of url index bits\n"
	, XT_MASSURL_VERSION);
}

static struct option opts[] = {
    { "type", 1, NULL, 1 },
    { "index", 1, NULL, 2 },
    { }
};

static const char *glbTypes[] = {"host", "dns", "http", "nothttp"};

static int parse(int c, char **argv, int invert, unsigned int *flags,
				  const void *entry, struct xt_entry_match **match)
{
	struct xt_massurl_info *info = (struct xt_massurl_info *)(*match)->data;
	char *pcIndex;
	unsigned int index;
	char ch ;
	unsigned int v;

	switch (c)
	{
	case 1:
		if (*flags & ARG_TYPE)
		{
			exit_error(PARAMETER_PROBLEM, "Can't specify multiple --type");
		}
		for (index = 0; index < sizeof(glbTypes) / sizeof(char *); index++)
		{
			if (strcmp(optarg, glbTypes[index]) == 0)
			{
				info->type = index;
				break;
			}
		}
		if (index >= sizeof(glbTypes) / sizeof(char *))
		{
			exit_error(PARAMETER_PROBLEM, "Only 'host', 'dns', 'http' or 'nothttp' available with --type");
		}
		*flags |= ARG_TYPE;
		break;

	case 2:
		if (*flags & ARG_INDEX)
		{
			exit_error(PARAMETER_PROBLEM, "Can't specify multiple --index");
		}
		if (strlen(optarg) > (XT_MASSURL_INDEX_BITS << 3))
		{
			exit_error(PARAMETER_PROBLEM,
					"Arg length error with --index, must not more than %d", XT_MASSURL_INDEX_BITS << 3);
		}

		memset(info->indexBits, 0, sizeof(info->indexBits));
		for (index = 0, pcIndex = optarg; *pcIndex != '\0'; pcIndex++, index++)
		{
			ch = *pcIndex;
			if (ch >= '0' && ch <= '9')
			{
				v = ch - '0'; 
			}
			else if (ch >= 'A' && ch <= 'F')
			{
				v = ch - 'A' + 10; 
			}
			else if (ch >= 'a' && ch <= 'f')
			{
				v = ch - 'a' + 10; 
			}
			else
			{
				exit_error(PARAMETER_PROBLEM,
							"Arg format error with --urls, must with only 0-9A-Fa-f");
			}
			info->indexBits[index >> 3] |= (v << ((index & 7) << 2));
		}
#ifdef XT_MASSURL_DEBUG
		printf("Mark url for:");
		for (index = 0; index < XT_MASSURL_MAX_ENTRY; index++)
		{
			if (XT_MASSURL_BIT_ISSET(info->indexBits, index))
			{
				printf(" %d", index);
			}
		}
		printf("\n");
#endif
		*flags |= ARG_INDEX;
		break;

	default:
		return 0;
	}

	return 1;
}

/* Final check */
static void check(unsigned int flags)
{
	if (!(flags & ARG_TYPE))
	{
		exit_error(PARAMETER_PROBLEM, "MASSURL match: You must specify `--type`");
	}
	if (!(flags & ARG_INDEX))
	{
		exit_error(PARAMETER_PROBLEM, "MASSURL match: You must specify `--index");
	}
}

/* Prints out the matchinfo. */
static void print(const void *ip, const struct xt_entry_match *match, int numeric)
{
	struct xt_massurl_info *info = match->data;

	printf("MASSURL match ");
	printf("--type %s ", glbTypes[info->type]);
}

static struct xtables_match massurl_match = {
    .name		= "massurl",
    .family		= AF_INET,
    .version	= IPTABLES_VERSION,
    .size		= XT_ALIGN(sizeof(struct xt_massurl_info)),
    .userspacesize	= XT_ALIGN(sizeof(struct xt_massurl_info)),
    .help		= help,
    .parse		= parse,
    .final_check= check,
    .print		= print,
	.extra_opts	= opts,
};


static struct xtables_match massurl_match6 = {
    .name		= "massurl",
    .family		= AF_INET6,
    .version	= IPTABLES_VERSION,
    .size		= XT_ALIGN(sizeof(struct xt_massurl_info)),
    .userspacesize	= XT_ALIGN(sizeof(struct xt_massurl_info)),
    .help		= help,
    .parse		= parse,
    .final_check= check,
    .print		= print,
	.extra_opts	= opts,
};

void _init(void)
{
	xtables_register_match(&massurl_match);
	xtables_register_match(&massurl_match6);
}
