#ifndef _COMMON_H_
#define _COMMON_H_

#include "config.h"
#include <stdio.h>
#include <c-ares/ares.h>

#define APE_BASEMEM 4096
#define __REV "2.0wip"

#define ape_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))
#define ape_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))

typedef struct _ape_global ape_global;

#include "events.h"


struct _ape_global {
	int basemem;
	void *ctx; /* public */
	struct _fdevent events;
	struct {
		ares_channel channel;
		struct {
			struct _ares_sockets *list;
			size_t size;
			size_t used;	
		} sockets;

	} dns;

	int is_running:1;
};


#endif
