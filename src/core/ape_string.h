#ifndef __APE_STRING_H_
#define __APE_STRING_H_

#include "ape_buffer.h"

typedef enum {
	ISO88591,
	UTF8
} string_encoding;


typedef struct {
	buffer b;
	int len;
	string_encoding encoding;
} string;

#endif
