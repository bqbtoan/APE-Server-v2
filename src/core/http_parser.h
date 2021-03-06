#ifndef __HTTP_PARSER_H_
#define __HTTP_PARSER_H_

#include <stdint.h>

typedef enum type {
	HTTP_PARSE_ERROR,
	HTTP_METHOD,
	HTTP_PATH_CHAR,
	HTTP_PATH_END,
	HTTP_QS_CHAR,
	HTTP_BODY_CHAR,
	HTTP_VERSION_MAJOR,
	HTTP_VERSION_MINOR,
	HTTP_HEADER_KEY,
	HTTP_HEADER_KEYC,
	HTTP_HEADER_VAL,
	HTTP_HEADER_VALC,
	HTTP_CL_VAL,
	HTTP_HEADER_END,
	HTTP_READY
} callback_type;


typedef enum states {
	GO,  /* start    */
	G1, /* GE */
	G2, /* GET */
	G3, /* GET  */
	P1, /* PO */
	P2, /* POS */
	P3, /* POST */
	P4, /* POST  */
	PT, /* path */
	H1, /* H */
	H2, /* HT */
	H3, /* HTT */
	H4, /* HTTP */
	H5, /* HTTP/ */
	H6, /* HTTP/[0-9] */
	H7, /* HTTP/[0-9]. */
	H8, /* HTTP/[0-9].[0-9] */
	EL, /* expect new line */
	ER, /* expect \n */
	HH, /* header key */
	HI, /* header value */
	C1, /* C */
	C2, /* O */
	C3, /* N */
	C4, /* T */
	C5, /* E */
	C6, /* N */
	C7, /* T */
	C8, /* - */
	C9, /* L */
	CA, /* E */
	CB, /* N */
	CC, /* G */
	CD, /* T */
	CE, /* H */
	CF, /* : */
	CG, /* space */
	CV, /* content-length value */
	E1, /* first hex in % path */
	E2, /* second hex in % path */
	FI, /* header is about to finish */
	BT, /* Body char */
	NR_STATES
} parser_state;

typedef int (*HTTP_parser_callback)(void* ctx, callback_type type, int value, uint32_t step);

typedef struct _http_parser {
	HTTP_parser_callback callback;  /* user callback function */
	void *ctx; 			/* user defined */
	uint32_t rx;			/* flag (32bit) (pass through states) */
	uint32_t step; 			/* char number */
	uint32_t cl; 			/* content-length */ /* TODO : store cl in rx */
	parser_state state; 		/* state */
} http_parser;


typedef enum methods {
	HTTP_GET,
	HTTP_POST
} http_method_t;



int parse_http_char(struct _http_parser *parser, const unsigned char c);


#define HTTP_PARSER_RESET(p) \
	do { \
		(p)->state = GO; \
		(p)->step 	= 0; \
		(p)->cl	= 0; \
		(p)->callback = NULL; \
		(p)->rx = 0; \
		(p)->ctx = NULL; \
	} while(0) \

#endif
