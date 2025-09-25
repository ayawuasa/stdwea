#include "p:\stdwea\gdefwea.h"
#include "p:\stdwea\strwea.h"
#include "p:\stdwea\memwea.h"
#include "p:\stdwea\iowea.h"

u4 main(u4 args, u1** vargs);

u8 __stdcall __WeaMain__() {
#ifdef MEMWEA_INCLUDE
	MAINMEMBANK = NewMemBank(1024 * 1024 * 128);
#ifdef IOWEA_INCLUDE
	STDOUT = FS_Open((void*)((s8)STD_OUTPUT_HANDLE), FS_SB);
	STDIN  = FS_Open((void*)((s8)STD_INPUT_HANDLE), FS_SB);
	// STDOUT = FS_Open("STDOUTLOG.txt", F_W|F_SW|F_SR|F_C|F_S);
	// STDERR  = FS_Open("STDERRLOG.txt", F_W|F_SW|F_SR|F_C|F_S);
	// STDIN  = FS_Open("STDINLOG.txt", F_W|F_SW|F_SR|F_C|F_S);
#endif// IOWEA_INCLUDED
	u1* cmd = (u1*)GetCommandLineA();
	u4 cmdlen = CstrLen(cmd);
	u4 args = 1;

	u1 inquotes = 0;

	for (u4 i = 0; i < cmdlen; i++) {
		if (cmd[i] == '"') { inquotes ^= 1; }
		else if (cmd[i] == ' ' && !inquotes) { while (cmd[i] && cmd[i] == ' ') { i++; } args++; }
	}
	
	u1** vargs = (u1**)Malloc(sizeof(u1*)*args + cmdlen);
	u1* arg = (u1*)vargs + sizeof(u1*) * args;
	u1* mem = (u1*)vargs;
	while (*cmd) {
		*vargs++ = arg;
		if (*cmd == '"') { inquotes = 1; cmd++; }
		if (!inquotes) { 
			while (*cmd && *cmd != ' ') { *arg++ = *cmd++; }
		}
		else {
			while (*cmd && *cmd != '"') { *arg++ = *cmd++; }
			cmd++;
			inquotes = 0;
		}
		*arg++ = 0;
		while (*cmd && *cmd == ' ') { cmd++; }
	}
	vargs -= args;

	// for (u8 i = 0; i < args; i++) {
	//	Print("%s\n", vargs[i]);
	// }
#endif// MEMWEA_INCLUDED

#if RELEASE == 0
	//timing =================
	u8 hertz;
	QueryPerformanceFrequency((LARGE_INTEGER*)&hertz);
	u8 c1, c2;
	FS_WriteFmt(STDOUT, (u1*)"start:\n\n");
	QueryPerformanceCounter((LARGE_INTEGER*)&c1);
	// code xdddd ============
#endif

#ifdef IOWEA_INCLUDE
#ifdef MEMWEA_INCLUDE
	u4 error = main((u4)args, vargs);
#endif// MEMWEA_INCLUDE
#else
	u4 error = main();
#endif// IOWEA_INCLUDE

#if RELEASE == 0
	//timing =================
	QueryPerformanceCounter((LARGE_INTEGER*)&c2);
	u8 ticks = c2 - c1;
	u8 seconds = ticks / hertz;
	u8 milisec = (ticks * 1000) / hertz - seconds * 1000;
	u8 microsec = (ticks * 1000 * 1000) / hertz - milisec * 1000 - seconds * 1000 * 1000;
	FS_WriteFmt(STDOUT, (u1*)"\n\ndone time:\nseconds: %u\nmiliseconds: %u\nmicroseconds: %u\nticks:%u\n", seconds, milisec, microsec, ticks);
#endif

#ifdef MEMWEA_INCLUDE
	FS_Flush(STDOUT);
	FS_Close(STDOUT);
	FS_Flush(STDIN);
	FS_Close(STDIN);
	Free(vargs);
	VirtualFree(MAINMEMBANK, 0, MEM_RELEASE);
#endif// MEMWEA_INCLUDE
	ExitProcess(error);
}