#ifndef DYNBUFFWEA_INCLUDE
#define DYNBUFFWEA_INCLUDE

#include "p:\stdwea\gdefwea.h"
#include "p:\stdwea\iowea.h"

typedef struct DYNBUFF DYNBUFF;
struct DYNBUFF {
	u4 occ;
	u4 cap;
	u4 tsize;
	u1* data;
};

DYNBUFF* DB_Create(u4 size, u4 typesize);

void DB_Free(DYNBUFF* buff);

void DB_Grow(DYNBUFF* buff);

void DB_Append(DYNBUFF* buff, void* source, u4 sources);

void DB_Push(DYNBUFF* buff, void* source);

void DB_Pop(DYNBUFF* buff, void* dest);

static inline u4 DB_ElementCount(DYNBUFF* buff);

#endif// DYNBUFFWEA_INCLUDE


#ifndef  DYNBUFFWEA_DEF
#define  DYNBUFFWEA_DEF

DYNBUFF* DB_Create(u4 size, u4 typesize) {
	DYNBUFF* tmp = (DYNBUFF*)Malloc(sizeof(DYNBUFF));
	tmp->data = Malloc(size * typesize);
	tmp->cap = size * typesize;
	tmp->occ = 0;
	tmp->tsize = typesize;
	return tmp;
}

void DB_Free(DYNBUFF* buff) {
	Free(buff->data);
	Free(buff);
}

void DB_Grow(DYNBUFF* buff) {
	u4 ncap = buff->cap << 1;
	if (ncap < buff->cap) { FatalError("Internal error: DB_GROW() underflow::buy a 64 bit pc bruh\n", 420); }
	u1* tmp = Malloc(ncap);
	memcpy(tmp, buff->data, buff->occ);
	Free(buff->data);
	buff->data = tmp;
}

void DB_Append(DYNBUFF* buff, void* source, u4 sources) {
	while (buff->cap <= buff->occ + sources) { DB_Grow(buff); }
	memcpy(buff->data + buff->occ, source, sources);
	buff->occ += sources;
}

void DB_Push(DYNBUFF* buff, void* source) {
	while (buff->cap <= buff->occ + buff->tsize) { DB_Grow(buff); }
	memcpy(buff->data + buff->occ, source, buff->tsize);
	buff->occ += buff->tsize;
}

void DB_Pop(DYNBUFF* buff, void* dest) {
	if (buff->occ < buff->tsize) {
		FatalError("Internal error: DB_Pop() underflow:: you poped too much buddy\n", 420);
	}
	buff->occ -= buff->tsize;
	if (dest) { memcpy(dest, buff->data + buff->occ, buff->tsize); }
}


static inline u4 DB_ElementCount(DYNBUFF* buff) {
	return buff->occ / buff->tsize;
}

#endif//DYNBUFFWEA_DEF