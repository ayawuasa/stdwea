#ifndef GDEFWEA_INCLUDE
#define GDEFWEA_INCLUDE

#define VAR_ARG(__STACK_BASE__, __STACK_POINTER__) void *__STACK_POINTER__ = (void*)((u1*)(&__STACK_BASE__) + sizeof(u8))
#define NXT_ARG(__VARIABLE__, __STACK_POINTER__) ut __VARIABLE__ = *(ut*)__STACK_POINTER__; (u1*)__STACK_POINTER__ += sizeof(u8);
#define STACKPTR(__TYPE__, __VAR__)	__TYPE__ __VAR__##ARR[1]; __VAR__ = (__TYPE__*)__VAR__##ARR;
#define BUFFELEMENTCOUNT(__BUFF__) (sizeof(__BUFF__)/sizeof(__BUFF__[0]))
#define offsetof(__TYPE__, __MEMBER__) ((u8)&(((__TYPE__*)0)->__MEMBER__))

int _fltused; // for some reason :'(
#undef min
#undef max

typedef char                   s1;
typedef short int              s2;
typedef int                    s4;
typedef long long int          s8;
typedef unsigned char          u1;
typedef unsigned short int     u2;
typedef unsigned int           u4;
typedef unsigned long long int u8;
typedef float                  r4;
typedef double                 r8;

typedef union {
	struct { u1 l; u1 h; };
	u2 w;
} REG16;

typedef union {
	struct { u1 l; u1 h; };
	u2 w;
	u4 d;
} REG32;

typedef union { u8* pi; r8* pf; u8 i; r8 f; u1* str; void* p; } ut;

#endif// GDEFWEA_INCLUDE
