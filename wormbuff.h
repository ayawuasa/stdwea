#ifndef WORMBUFF_INCLUDE
#define WORMBUFF_INCLUDE
#include "p:\stdwea\gdefwea.h"
#include "p:\stdwea\memwea.h"

typedef struct WORMBUFF WORMBUFF;
struct  WORMBUFF {
	u1* b;
	u4 head; // left index  [----------], insert a -> a[---------]
	u4 tail; // right index [----------], insert a -> [---------]a
	u4 size;
};

WORMBUFF* WormBuffCreate(u4 size);

void WormBuffFree(WORMBUFF* w);

// moves the head and tail of the worm buffer into the index
void WormBuffMov(WORMBUFF* wbuff, u4 index);

// dumps the whole worm buffer into a field of a structure array
void WormBuffDumpIntoStruct(WORMBUFF* wbuff, void* buff, u4 structSize, u4 fieldOffset);

// dumps the whole worm buffer into a contiguous array returns number of bytes dumped
u4 WormBuffDump(WORMBUFF* wbuff, u1* buff);

#endif

#ifdef CONSOLE_DEF
#undef CONSOLE_DEF

WORMBUFF* WormBuffCreate(u4 size) {
	WORMBUFF* w = (WORMBUFF*)Malloc(sizeof(WORMBUFF));
	w->b = Malloc(size);
	w->size = size;
	w->head = 0;
	w->tail = size - 1;
	return w;
}

void WormBuffFree(WORMBUFF* w) {
	Free(w->b);
	Free(w);
}

// moves the head and tail of the worm buffer into the index
void WormBuffMov(WORMBUFF* wbuff, u4 index) {
	while (index < wbuff->head) {
		wbuff->b[wbuff->tail--] = wbuff->b[--wbuff->head];
	}
	while (wbuff->head < index) {
		wbuff->b[wbuff->head++] = wbuff->b[++wbuff->tail];
	}
}

// dumps the whole worm buffer into a field of a structure array
void WormBuffDumpIntoStruct(WORMBUFF* wbuff, void* buff, u4 structSize, u4 fieldOffset) {
	buff = (u1*)buff + fieldOffset;
	for (u4 i = 0; i < wbuff->head; i++) {
		*((u1*)buff) = wbuff->b[i];
		(u1*)buff += structSize;
	}
	for (u4 i = 0; i < wbuff->size - wbuff->tail - 1; i++) {
		*((u1*)buff) = wbuff->b[i + wbuff->tail + 1];
		(u1*)buff += structSize;
	}
}

// dumps the whole worm buffer into a contiguous array
u4 WormBuffDump(WORMBUFF* wbuff, u1* buff) {
	for (u4 i = 0; i < wbuff->head; i++) {
		buff[i] = wbuff->b[i];
	}
	for (u4 i = 0; i < wbuff->size - wbuff->tail - 1; i++) {
		buff[wbuff->head + i] = wbuff->b[i + wbuff->tail + 1];
	}
	return  wbuff->head + wbuff->size - wbuff->tail-1;
}

#endif