#ifndef IOWEA_INCLUDE
#define IOWEA_INCLUDE

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "p:\stdwea\gdefwea.h"
#include "p:\stdwea\memwea.h"
#include "p:\stdwea\strwea.h"

void ListFiles(u1* dir, u1* pattern);

void DelateDir(u1* dir);

u1* LoadFile(const u1* dir);

typedef struct FS FS;
struct FS {
	HANDLE h;
	u1* d; // directory
	u8 fs; // file size
	u8 fi; // file index

	u8  si;   // stream inedex
	u1* sb;   // stream buffer
};

FS* STDOUT;
FS* STDERR;
FS* STDIN;

typedef enum FILE_STREAM_ATT FILE_STREAM_ATT;
enum FILE_STREAM_ATT{
	FS_SD = FILE_SHARE_DELETE << 3,
	FS_SW = FILE_SHARE_WRITE << 3, 
	FS_SR = FILE_SHARE_READ << 3,
	FS_SA = FS_SD | FS_SW | FS_SR,
	FS_SB = 0x10, // FILE is ALSO A STREAM
	FS_OE = OPEN_EXISTING,
	FS_OA = OPEN_ALWAYS,
	FS_CA = CREATE_ALWAYS,
	FS_R  = GENERIC_READ,
	FS_W  = GENERIC_WRITE,
	FS_RW = FS_W | FS_R
};

u4 Print(u1* str, ...);

void FatalError(const u1* msg, u4 error);

u4 GetLine();

FS* FS_Open(const u1* dir, u8 att);

void FS_Close(FS* fs);

u4 FS_Write(FS* fs, const void* buff, u8 size);

u4 FS_WriteFmt(FS* f, u1* str, ...);

u4 FS_Read(FS* fs, const void* buff, u8 size);

void FS_WriteFmtb(FS* f, const void* str, ...);

void FS_Writeb(FS* f, const void* buff, u8 size);

void FS_Flush(FS* f);

#endif// IOWEA_INCLUDE

#ifndef  IOWEA_DEF
#define  IOWEA_DEF

#define STREAM_BUFF_CAP 4096
#define TEMP_BUFF_CAP   4096

// CreateDirectoryA("wea", 0);
void ListFiles(u1* dir, u1* pattern) {
	WIN32_FIND_DATA ffd;
	u1 buff[255];
	if (!pattern) { pattern = (u1*)"*"; }
	CstrFmt(buff, (u1*)"%s\\%s", dir, pattern);

	HANDLE hFind = FindFirstFileA((LPCSTR)dir, &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		u1 index = 0;
		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { FS_WriteFmtb(STDOUT, "<DIR> %s\n", ffd.cFileName); }
			else { FS_WriteFmtb(STDOUT, (const void*)"%ur :%sl : bytes:%u\n", index++, (u8)5, ffd.cFileName, (u8)32, (u8)ffd.nFileSizeLow | (((u8)ffd.nFileSizeHigh) << 32));
		}
		} while (FindNextFileA(hFind, &ffd) != 0);
		FS_Flush(STDOUT);
		FindClose(hFind);
	}
}

void DelateDir(u1* dir) {
	WIN32_FIND_DATA ffd;
	u1 buff[255];
	u8 size = CstrFmt(buff, (u1*)"%s\\%%s", dir) - 2;
	HANDLE hFind = FindFirstFileA((LPCSTR)dir, &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { DelateDir((u1*)ffd.cFileName); }
			else { CstrFmt(buff + size, buff + size, ffd.cFileName); DeleteFileA((LPCSTR)buff); }
		} while (FindNextFileA(hFind, &ffd) != 0);
		FindClose(hFind);
	}
	RemoveDirectoryA((LPCSTR)dir);
}

u1* LoadFile(const u1* dir) {
	HANDLE h = CreateFileA((LPCSTR)dir, GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	u4 size = 0;
	GetFileSizeEx(h, (void*)&size);
	u1* buff = Malloc(size+1);
	ReadFile(h, buff, size, 0, 0);
	buff[size] = 0;
	CloseHandle(h);
	return buff;
}

// FILE_STREAM =====================

u4 Print(u1* str, ...) {
	VAR_ARG(str, stack);
	u1 buff[TEMP_BUFF_CAP];
	u4 wb = 0;
	WriteFile(STDOUT->h, buff, CstrFmtS(buff, str, stack), (void*)&wb, 0);
	return wb;
}

void FatalError(const u1* msg, u4 error) {
	Print((u1*)msg);
	ExitProcess(error);
}

u4 GetLine() {
	ReadFile(STDIN->h, STDIN->sb, STREAM_BUFF_CAP, (void*)&STDIN->si, 0);
	return STDIN->si -= 2;
}

FS* FS_Open(const u1* dir, u8 att) {
	FS* f = (FS*)Malloc(sizeof(FS));
	memset(f, 0, sizeof(FS));

	f->d = (u1*)dir;
	if (!att) {	att = FS_SA | FS_OE | FS_RW; }
	if (att & FS_SB) { f->sb = Malloc(STREAM_BUFF_CAP); }

	switch ((u8)dir) {
		case STD_INPUT_HANDLE: {
			f->h = GetStdHandle(STD_INPUT_HANDLE);
		} break;
		case STD_OUTPUT_HANDLE: {
			f->h = GetStdHandle(STD_OUTPUT_HANDLE);
		} break;
		case STD_ERROR_HANDLE: {
			f->h = GetStdHandle(STD_ERROR_HANDLE);
		} break;
		default: {
			f->h = CreateFileA(dir, att & (GENERIC_READ | GENERIC_WRITE), (att >> 3) & 0x07, 0, att & 07, FILE_ATTRIBUTE_NORMAL, 0);
			GetFileSizeEx(f->h, (LARGE_INTEGER*)&f->fs);
		}
	}
	return f;
}

void FS_Close(FS* fs) {
	if (fs->h != GetStdHandle(STD_INPUT_HANDLE) && fs->h != GetStdHandle(STD_OUTPUT_HANDLE) && fs->h != GetStdHandle(STD_ERROR_HANDLE)) {
		u1 buff[256];
		u4 written;
		CloseHandle(fs->h);
	}
	if (fs->sb) { Free(fs->sb); }
	Free(fs);
}

u4 FS_Read(FS* fs, const void* buff, u8 size) {
	OVERLAPPED o = { 0 };
	o.Pointer = (PVOID)fs->fi;
	u4 bytesRead = 0;
	ReadFile(fs->h, (void*)buff, size, (void*)&bytesRead, &o);
	fs->fi += bytesRead;
	return bytesRead;
}

u4 FS_Write(FS* fs, const void* buff, u8 size) {
	u4 bytesWritten;
	OVERLAPPED o = { 0 };
	o.Pointer = (PVOID)fs->fi;
	WriteFile(fs->h, buff, size, (void*)&bytesWritten, &o);
	fs->fs += size;
	fs->fi += size;
	return bytesWritten;
}

u4 FS_WriteFmt(FS* fs, u1* str, ...) {
	VAR_ARG(str, stack);
	u1 buff[TEMP_BUFF_CAP];
	u4 bytesWritten = 0;
	WriteFile(fs->h, buff, CstrFmtS(buff, str, stack), (void*)&bytesWritten, 0);
	return bytesWritten;
}

void FS_Writeb(FS* fs, const void* buff, u8 size) {
	u8 p1 = STREAM_BUFF_CAP - fs->si;
	if (p1 < size) {
		memcpy(fs->sb + fs->si, buff, p1);
		WriteFile(fs->h, fs->sb, STREAM_BUFF_CAP, 0, 0);
		memcpy(fs->sb, ((u1*)buff) + p1, fs->si = size - p1);
	}
	else {
		memcpy(fs->sb + fs->si, buff, size);
		fs->si += size;
	}
}

void FS_WriteFmtb(FS* fs, const void* str, ...) {
	VAR_ARG(str, stack);
	u1 buff[TEMP_BUFF_CAP];
	FS_Writeb(fs, buff, CstrFmtS(buff, (u1*)str, stack));
}

void FS_Flush(FS* fs) {
	WriteFile(fs->h, fs->sb, fs->si, 0, 0);
	fs->si = 0;
}

#undef STREAM_BUFF_CAP
#undef TEMP_BUFF_CAP

#endif//  IOWEA_DEF