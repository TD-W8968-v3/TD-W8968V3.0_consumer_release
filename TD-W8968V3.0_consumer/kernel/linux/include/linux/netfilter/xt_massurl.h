#ifndef __XT_MASSURL_H__
#define __XT_MASSURL_H__

#include <linux/types.h>

/* #define XT_MASSURL_DEBUG */

#define XT_MASSURL_VERSION		"0.0.1"
#define XT_MASSURL_MAX_LENGTH	33
#define XT_MASSURL_MAX_ENTRY	200
#define XT_MASSURL_INDEX_BITS	((XT_MASSURL_MAX_ENTRY + 31) >> 5)

#define XT_MASSURL_BIT_ISSET(set, index) (set[index >> 5] & (1 << (index & 0x1F)))

struct xt_massurl_url_info
{
	__u16 index;
	char addr[XT_MASSURL_MAX_LENGTH];
};

enum {
	XT_MASSURL_MATCH_HOST = 0,
	XT_MASSURL_MATCH_DNS,
	XT_MASSURL_MATCH_HTTP,
	XT_MASSURL_MATCH_NOTHTTP
};

struct xt_massurl_info
{
	__u8 type;		/* address type */
	unsigned indexBits[XT_MASSURL_INDEX_BITS];
};

#endif	/* __XT_MASSURL_H__ */
