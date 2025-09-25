#ifndef MEMWEA_INCLUDE
#define MEMWEA_INCLUDE
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "p:\stdwea\gdefwea.h"

#define MAINMEMBANK __main_mem_bank__
// #define Malloc(BYTECOUNT)           HeapAlloc(HEAP, HEAP_ZERO_MEMORY, BYTECOUNT)
// #define Realloc(POINTER, BYTECOUNT) HeapReAlloc(HEAP, HEAP_ZERO_MEMORY, POINTER, BYTECOUNT)
// #define Free(POINTER)               HeapFree(HEAP, 0, POINTER);

typedef struct MEMCHUNK MEMCHUNK;
struct MEMCHUNK {
	u8 size;
	MEMCHUNK* nxt;
	MEMCHUNK* prv;
};

typedef struct MEMBANK MEMBANK;
struct MEMBANK {
	u1* base; // base pointer
	u1* nxt;  // next pointer to allocated
	u8 ps; // page size
	u8 pt; // page total
	u8 fp; // free page
	MEMCHUNK* fc; // freechunk
};

MEMBANK* __main_mem_bank__;

#pragma function(memset)
void *memset(void *dest, int c, size_t count);

#pragma function(memcpy)
void *memcpy(void *dest, const void *src, size_t count);

#pragma function(memcmp)
int memcmp(const void* ptr1, const void* ptr2, size_t num);

MEMBANK* NewMemBank(u8 MaxMemCap);

#define MEMWEA_DEBUG 0
#if MEMWEA_DEBUG == 0
u1* Malloc(u8 size);

void Free(void* ptr);
#else
#define Malloc(__SIZE__) Debug_Malloc(__FILE__, __LINE__, __SIZE__)
#define Free(__PTR__) Debug_Free(__FILE__, __LINE__, __PTR__)

void Debug_Free(u1* file, u8 line, void* ptr);
u1* Debug_Malloc(u1* file, u8 line, u8 size);

void PrintMemChunks();
#endif


#endif// MEMWEA_INCLUDE

#ifndef  MEMWEA_DEF
#define  MEMWEA_DEF

void *memset(void *dest, int c, size_t count){
    while (count--){ *((u1*)dest)++ = (u1)c; }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t count){
    while (count--){ *((u1*)dest)++ = *((u1*)src)++; }
	return dest;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num) {
	while (num--) { if (*((u1*)ptr1)++ != *((u1*)ptr2)++) { return 0; }	}
	return 1;		  
}

#if MEMWEA_DEBUG == 0
u1* Malloc(u8 size) {
	MEMBANK* mb = MAINMEMBANK;

	if (size < sizeof(MEMCHUNK) - sizeof(u8)) { size = sizeof(MEMCHUNK); }
	else { size += ((-size) & 0x07) + 8; }

	MEMCHUNK* fc = mb->fc;
	MEMCHUNK* cc = fc;

	do {
		cc = fc;
		if (fc->size >= size) {
			fc->size -= size;
			if (fc->size <= sizeof(MEMCHUNK)) {
				if (fc->prv && (fc->prv->nxt = fc->nxt)) { fc->nxt->prv = fc->prv; }
				else if ((mb->fc = fc->nxt)) { fc->nxt->prv = 0; }
				return (u1*)&fc->nxt;
			}
			MEMCHUNK* ptr = (MEMCHUNK*)((u1*)fc + fc->size);
			ptr->size = size;
			return (u1*)&ptr->nxt;
		}
	} while (fc = cc->nxt);

	u4 newSize = mb->ps;
	u4 pt = 1; // page total

	while (newSize < size) {
		newSize += mb->ps;
		pt++;
		if (pt > mb->fp) { return (u1*)1; }
	}

	mb->fp -= pt;
	MEMCHUNK* nc = (MEMCHUNK*)mb->nxt;

	mb->nxt = (u1*)((u8)VirtualAlloc((void*)mb->nxt, newSize, MEM_COMMIT, PAGE_READWRITE) + newSize);

	nc->size = newSize;

	if (cc) {
		if ((u8)((u1*)cc + cc->size) == (u8)nc) {
			cc->size += nc->size;
			nc = cc;
		}
		else {
			cc->nxt = nc;
			nc->prv = cc;
		}
	}

	u8 diff = nc->size - size;
	if (diff <= sizeof(MEMCHUNK)) {
		if (nc->prv) { nc->prv->nxt = 0; }
		else { mb->fc = 0; }
		return (u1*)&nc->nxt;
	}
	nc->size -= size;
	MEMCHUNK* ptr = (MEMCHUNK*)((u1*)nc + nc->size);
	ptr->size = size;
	return (u1*)&ptr->nxt;
}

void Free(void* ptr){
	MEMBANK* mb = MAINMEMBANK;
	MEMCHUNK* mc = (MEMCHUNK*)((u1*)ptr - offsetof(MEMCHUNK, nxt));
	MEMCHUNK* nxt = mb->fc;
	
	mc->nxt = 0;
	mc->prv = 0;
	
	if (!nxt) {	mb->fc = mc; return; }

	while (mc > nxt) {
		if (!nxt->nxt) {
			nxt = (MEMCHUNK*)((u8)nxt ^ (u8)mc);
			mc  = (MEMCHUNK*)((u8)mc  ^ (u8)nxt);
			nxt = (MEMCHUNK*)((u8)nxt ^ (u8)mc);
			goto lb0;
		}
		nxt = nxt->nxt;
	}
	if (nxt->prv) {
		MEMCHUNK* prv = nxt->prv;

		if ((u8)((u1*)prv + prv->size) == (u8)mc) {
			prv->size += mc->size;
			mc = prv;
		}
		else {
			mc->prv = prv;
			prv->nxt = mc;
		}

	}
	else { mb->fc = mc; }
lb0:if ((u8)((u1*)mc + mc->size) == (u8)nxt) {
		mc->size += nxt->size;
		mc->nxt = nxt->nxt;
		if (nxt->nxt) { nxt->nxt->prv = mc; }
	}
	else {
		mc->nxt = nxt;
		nxt->prv = mc;
	}
}

#endif

MEMBANK* NewMemBank(u8 MaxMemCap) {
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	u8 maxMemSize = (MaxMemCap / sysInfo.dwPageSize + 1) * sysInfo.dwPageSize;

	MEMBANK* ptr = (MEMBANK*)VirtualAlloc(0, maxMemSize, MEM_RESERVE, PAGE_NOACCESS);
	ptr = (MEMBANK*)VirtualAlloc((void*)ptr, sysInfo.dwPageSize, MEM_COMMIT, PAGE_READWRITE);

	ptr->base = (u1*)((u8)ptr + sizeof(MEMBANK));
	ptr->nxt = ((u1*)ptr) + sysInfo.dwPageSize;
	ptr->ps = sysInfo.dwPageSize;
	ptr->pt = maxMemSize / sysInfo.dwPageSize;
	ptr->fp = ptr->pt - 1;
	ptr->fc = (MEMCHUNK*)ptr->base;
	ptr->fc->size = sysInfo.dwPageSize - sizeof(MEMBANK);
	return ptr;
}

#if MEMWEA_DEBUG
#include "p:\stdwea\iowea.h"
FS* __debug_memwea_log__ = 0;
u8 __debug_malloc_calls__ = 0;
u8 __debug_free_calls__ = 0;

u1* Debug_Malloc(u1* file, u8 line, u8 size) {
	__debug_malloc_calls__++;
	u8 oldSize = size;

	MEMBANK* mb = MAINMEMBANK;
	if (size  < sizeof(MEMCHUNK) - sizeof(u8)) { size = sizeof(MEMCHUNK); } // cambio
	else { size += ((-size) & 0x07) + 8; } ///cambio

	FS_WriteFmt(__debug_memwea_log__, "\n\nMalloc:%u:%s:%u:s:%u, rs:%u, 0x%xr0\n",
	__debug_malloc_calls__, file, line, oldSize, size, size, (u8)11);
	PrintMemChunks(__LINE__);

	MEMCHUNK* fc = mb->fc;
	MEMCHUNK* cc = fc;

	if (cc) {
		do {
			cc = fc;
			if (fc->size >= size) {
				u8 diff = fc->size - size;
				if (diff <= sizeof(MEMCHUNK)) {////cambio
					if (fc->prv && (fc->prv->nxt = fc->nxt)) {////cambio
							fc->nxt->prv = fc->prv;////cambio
					}
					else if (mb->fc = fc->nxt) { fc->nxt->prv = 0; }
					FS_WriteFmt(__debug_memwea_log__, "ret ptr:0x%xr0\n", (u1*)&fc->nxt, (u8)11);
					PrintMemChunks(__LINE__);
					return (u1*)&fc->nxt;
				}
				fc->size -= size;
				MEMCHUNK* ptr = (MEMCHUNK*)((u1*)fc + fc->size);
				ptr->size = size;
				FS_WriteFmt(__debug_memwea_log__, "ret ptr:0x%xr0\n", (u1*)&ptr->nxt, (u8)11);
				PrintMemChunks(__LINE__);
				return (u1*)&ptr->nxt;
			}
		} while (fc = cc->nxt);
	}

	u4 newSize = mb->ps;
	u4 pt = 1; // page total

	while (newSize < size) {
		newSize += mb->ps;
		pt++;
		if (pt > mb->fp) { return (u1*)1; }
	}

	mb->fp -= pt;
	MEMCHUNK* nc = (MEMCHUNK*)mb->nxt;

	mb->nxt = (u1*)((u8)VirtualAlloc((void*)mb->nxt, newSize, MEM_COMMIT, PAGE_READWRITE) + newSize);

	nc->size = newSize;

	if (cc) {
		if ((u8)((u1*)cc + cc->size) == (u8)nc) {
			cc->size += nc->size;
			nc = cc;
		}
		else {
			cc->nxt = nc;
			nc->prv = cc;
		}
	}
	else {
		mb->fc = nc;
	}

	u8 diff = nc->size - size;
	if (diff <= sizeof(MEMCHUNK)) {
		if (nc->prv) { nc->prv->nxt = 0; }
		else { mb->fc = 0; }
		FS_WriteFmt(__debug_memwea_log__, "ret ptr:0x%xr0\n", (u1*)&nc->nxt, (u8)11);
		PrintMemChunks(__LINE__);
		return (u1*)&nc->nxt;
	}
	nc->size -= size;
	MEMCHUNK* ptr = (MEMCHUNK*)((u1*)nc + nc->size);
	ptr->size = size;
	FS_WriteFmt(__debug_memwea_log__, "ret ptr:0x%xr0\n", (u1*)&ptr->nxt, (u8)11);
	PrintMemChunks(__LINE__);
	return (u1*)&ptr->nxt;
}

void Debug_Free(u1* file, u8 line, void* ptr) {
	__debug_free_calls__++;


	MEMBANK* mb = MAINMEMBANK;
	MEMCHUNK* mc = (MEMCHUNK*)((u1*)ptr - offsetof(MEMCHUNK, nxt));
	MEMCHUNK* nxt = mb->fc;
	
	FS_WriteFmt(__debug_memwea_log__, "\n\nFree:%u:%s:%u: ptr:0x%xr0\n", __debug_free_calls__, file, line, ptr, (u8)11);
	FS_WriteFmt(__debug_memwea_log__, 
		"mc:0x%xr0, size:%u:0x%xr0\nnxt:0x%xr0, prv:0x%xr0\n",
		mc,(u8)16, 
		mc->size,
		mc->size, (u8)11,
		mc->nxt, (u8)11,
		mc->prv, (u8)11);
	PrintMemChunks(__LINE__);
	
	mc->nxt = 0;
	mc->prv = 0;
	
	if (!nxt) {
		mb->fc = mc;
		PrintMemChunks(__LINE__);
		return;
	}
	
	while (mc > nxt) {
		if (!nxt->nxt) {
			(u8)nxt ^= (u8)mc;
			(u8)mc ^= (u8)nxt;
			(u8)nxt ^= (u8)mc;
			goto lb0;
		}
		nxt = nxt->nxt;
	}
	if (nxt->prv) {
		MEMCHUNK* prv = nxt->prv;

		if ((u8)((u1*)prv + prv->size) == (u8)mc) {
			prv->size += mc->size;
			mc = prv;
		}
		else {
			mc->prv = prv;
			prv->nxt = mc;
		}

	}
	else { mb->fc = mc; }
lb0:if ((u8)((u1*)mc + mc->size) == (u8)nxt) {
		mc->size += nxt->size;
		mc->nxt = nxt->nxt;
		if (nxt->nxt) { nxt->nxt->prv = mc; }
	}
	else {
		mc->nxt = nxt;
		nxt->prv = mc;
	}
	PrintMemChunks(__LINE__);
}

void PrintMemChunks(u8 LINE) {
	MEMBANK* mb = MAINMEMBANK;
	MEMCHUNK* nxt = mb->fc;
	FS_WriteFmt(__debug_memwea_log__, "fp:0x%xr0=======%u=====\n", mb->fp, (u8)8, LINE);
	while (nxt) {
		FS_WriteFmt(__debug_memwea_log__, "0x%xr0: n:0x%xr0 p:0x%xr0 s:%u:0x%xr0\n",
			(u8)nxt, (u8)11,
			(u8)nxt->nxt, (u8)11,
			(u8)nxt->prv, (u8)11,
			(u8)nxt->size,
			(u8)nxt->size, (u8)11);
		
		u8 buff[] = { (u8)nxt, (u8)nxt->nxt, (u8)nxt->prv, (u8)nxt->size };
		nxt = nxt->nxt;
	}
	FS_WriteFmt(__debug_memwea_log__, "=========================\n");
}

#endif


#endif// MEMWEA_DEF