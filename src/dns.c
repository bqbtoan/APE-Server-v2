#include <c-ares/ares.h>
#include <netdb.h>
#include <stdlib.h>
#include "common.h"
#include "dns.h"
#include "events.h"

/* gcc *.c -I../deps/ ../deps/c-ares/.libs/libcares.a -lrt */

struct _ape_dns_cb_argv {
	ape_global *ape;
	ape_gethostbyname_callback callback;
	const char *origin;
};

static void ares_socket_cb(void *data, int s, int read, int write)
{
	ape_global *ape = data;
	int i, f = 0;
	
	for (i = 0; i < ape->dns.sockets.size; i++) {
		if (!f && ape->dns.sockets.list[i].s.fd == 0) { /* TODO memset with 0 */
			f = i;
		} else if (ape->dns.sockets.list[i].s.fd == s) {
			/* Modify or delete the object (+ return) */
		}
	}
	
	ape->dns.sockets.list[f].s.fd 	= s;
	ape->dns.sockets.list[f].s.type = APE_DELEGATE;
	
	events_add(s, &ape->dns.sockets.list[f], EVENT_READ|EVENT_WRITE, ape);
	
	printf("Socket %i %i %i\n", s, read, write);
}

int ape_dns_init(ape_global *ape)
{
	struct ares_options opt;
	int optmask = 0;
	
	if (ares_library_init(ARES_LIB_INIT_ALL) != 0) {
		return -1;
	}
	
	opt.sock_state_cb 	= ares_socket_cb;
	opt.sock_state_cb_data 	= ape;
	
	if (ares_init_options(&ape->dns.channel, &opt, 0x00 | ARES_OPT_SOCK_STATE_CB) != ARES_SUCCESS) { /* At the moment we only use one dns channel */
		return -1;
	}
	
	ape->dns.sockets.list 	= malloc(sizeof(struct _ares_sockets) * 32);
	ape->dns.sockets.size 	= 32;
	ape->dns.sockets.used	= 0;
}

void ares_gethostbyname_cb(void *arg, int status, int timeout, struct hostent *host)
{
	struct _ape_dns_cb_argv *params = arg;
	char ret[46];

	if (status == ARES_SUCCESS) {
		inet_ntop(host->h_addrtype, *host->h_addr_list, ret, sizeof(ret)); /* only return the first h_addr_list element */
		params->callback(ret);
	}
	
	free(params);
}

void ape_gethostbyname(const char *host, ape_gethostbyname_callback callback, ape_global *ape)
{
	struct in_addr addr4;
	
	if (inet_pton(AF_INET, host, &addr4) == 1) {
		callback(host);
	} else {
		struct _ape_dns_cb_argv *cb 	= malloc(sizeof(*cb));
		cb->ape 			= ape;
		cb->callback 			= callback;
		cb->origin			= host;
		ares_gethostbyname(ape->dns.channel, host, AF_INET, ares_gethostbyname_cb, cb);

	}
}

