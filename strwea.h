#ifndef STRWEA_INCLUDE
#define STRWEA_INCLUDE
#include "p:\stdwea\gdefwea.h"



u8 Cstr2Int(u1* str);

u4 Int2Cstr(u1* buff, u8 i, u1 tsize, u1 sign, u1 base);

u8 Float2Cstr(u1* str, r8 f);

r8 Cstr2Float(u1* str);

u4 CstrFmt(u1* buff, u1* str, ...);

u4 CstrLen(u1* str);

u1 CstrCmp(u1* str1, u1* str2);

u1 CstrCmpS(u1* str1, u1* str2, u4 size);

inline u1 CharIsSpace(u1 c);
inline u1 CharIsNL(u1 c);
inline u1 CharIsAlp(u1 c);
inline u1 CharIsNum(u1 c);
inline u1 CharIsNumX(u1 c);
inline u1 CharIsAlpNum(u1 c);
inline u1 CharIsPrintable(u1);

typedef struct STR STR;
struct STR {
	u8 s;
	u1* d;
};

#endif// STRWEA_INCLUDE

#ifndef  STRWEA_DEF
#define  STRWEA_DEF

u8 Cstr2Int(u1* str) {
	u8 v = 0;
	if (*str == '0') {
		switch (*(str + 1) & 0xDF) {
			case 'B':{ str += 2; while ((u1)(*str - '0') < 2 ) { v = (v << 1) + *str++ - '0'; } } break;
			case 'O':{ str += 2; while ((u1)(*str - '0') < 8 ) { v = (v << 3) + *str++ - '0'; } } break;
			case 'D':{ str += 2; while ((u1)(*str - '0') < 10) { v = (v * 10) + *str++ - '0'; } } break;
			case 'X':{
				str += 2;
				while (CharIsNumX(*str)) {
					u1 c = *str++;
					if ((u1)((c & 0xDF) - 'A') < 6) { c = (c & 0xDF) - 7; }
					v = (v << 4) + c - '0';
				}
			} break;
		}
	}
	else { while ((u1)(*str - '0') < 10) { v = v * 10 + *str++ - '0'; }
	}
	return v;
}

// tsize must be 1,2,4,8
u4 Int2Cstr(u1* buff, u8 i, u1 tsize,u1 sign, u1 base) {
	u1* b = buff;
	u8 mask = ~((u8)0) >> ((8 - tsize) << 3);
	i &= mask;
	if (sign && (i & ((mask >> 1) ^ mask))) { *b++ = '-'; i = (~i + 1) & mask; }
	else { sign = 0; }
	
	switch (base) {
		case 0x10: { do { *b++ = "0123456789ABCDEF"[i & 0x0F]; i >>= 4; } while (i); } break;
		case 0x02: { do { *b++ = '0' + (i & 0x01); i >>= 1; } while (i); } break;
		case 0x08: { do { *b++ = '0' + (i & 0x07); i >>= 3; } while (i); } break;
		default:   { do { *b++ = '0' + (i % base); i /= base; } while (i); } break;
	}
	*b = '\0';
	u1* bh = b, *bt = buff + sign;
	
	while (bt + 1 < bh) { 
		u1 c = *bt;
		*bt++ = *--bh;
		*bh = c;
	}
	
	return (u4)(b-buff) + 1;
}

u8 Float2Cstr(u1* buff, r8 f) {
	u1 *b = buff;
	u8 w = 0;
	if (f < 0) { w++; *b++ = '-'; f = -f; }
	u8 i = (u8)f;
	u1* bt = b;
	
	f -= i;
	do { w++; *b++ = '0' + (i % 10); i /= 10; } while (i);
	u1* bh = b;
	while (bt + 1 < bh) { u1 c = *bt; *bt++ = *--bh; *bh = c; }
	
	if (f != 0) {
		w++;
		*b++ = '.';
		for (u1 p = 0; p < 15; p++) { 
			w++; 
			f -= i;
			f *= 10;
			i = (u8)f;
			*b++ = '0' + i;
		}
	}
	*b = 0;
	return ++w;
}

r8 Cstr2Float(u1* buff) {
	r8 f = 0;
	while (CharIsNum(*buff)){ f = f * 10 + *buff++ - '0'; }
	if (*buff++ == '.') {
		r8 frag = 0;
		r8 dig = 1;
		while (CharIsNum(*buff)) { dig *= 10; frag = frag * 10 + *buff++ - '0'; }
		f += frag / dig;
	}
	return f;
}	

/*
	% size, mode, padding, padding character
	size of data type in bytes
	mode
	c = char, b = binary, o = octal, x = hex, i =signed int, u = unsigned int, f = float
	s = cstring (null terminated), $ = partial string (the length should be past as an argument) ex, CstrFmt(b, "%$", str, 4);
	padding
	r pading right
	l pading left
	padding character
	the character to use in case of not fully fill the padding area
*/
u4 CstrFmtS(u1* buff, u1* str, void* stack) {
	u8 length = (u8)buff;
lp0:while (*str) {
		if (*str == '%') {
			if (*++str != '%') {
				NXT_ARG(var, stack);
				u4 w = 0;
				u1 mode = *str++;
				u1 tsize = 8;
				if (CharIsNum(mode)) {
					tsize = mode - '0';
					mode = *str++;
				}
				switch (mode) {
					case 's': {
						while (*var.str) { buff[w++] = *var.str++; }
					} break;
					case '$': {
						NXT_ARG(L, stack);
						while (L.i--) {
							buff[w++] = *var.str++;
						}
					} break;
					case 'c': { buff[w++] = var.i; } break;
					case 'b': { w = Int2Cstr(buff, var.i, tsize, 0, 0x02) - 1; } break;
					case 'o': { w = Int2Cstr(buff, var.i, tsize, 0, 0x08) - 1; } break;
					case 'x': { w = Int2Cstr(buff, var.i, tsize, 0, 0x10) - 1; } break;
					case 'i': { w = Int2Cstr(buff, var.i, tsize, 1, 0x0A) - 1; } break;
					case 'u': { w = Int2Cstr(buff, var.i, tsize, 0, 0x0A) - 1; } break;
					case 'f': { w = Float2Cstr(buff, var.f) - 1;			   } break;
					default:{ *((u4*)buff) = ':vni'; buff += 4; *buff++ = '('; *buff++ = mode; *buff++ = ')'; goto lp0; }
				}
				switch (*str) {
					case 'r': {
						str++;
						NXT_ARG(ps, stack);
						u1 pad = *str++;
						u1* b = buff + w;
						u1* p = buff + (u4)ps.i-1;
						w = (u4)ps.i;
						do { *p-- = *--b; } while (buff < b);
						while (buff <= p) { *p-- = pad; }
					}break;
					case 'l':{
						str++;
						NXT_ARG(ps, stack);
						u1 pad = *str++;
						u1* b = buff + w;
						for (u4 i = 0; i < (u4)ps.i -w; i++) { *b++ = pad; }
						w = (u4)ps.i;
					}break;
				}
				buff += w;
				goto lp0;
			}
		}
		*buff++ = *str++;
	}
	*buff = 0;
	return (u4)((u8)buff - length);
}

u4 CstrFmt(u1* buff, u1* str, ...) {
	VAR_ARG(str, stack);
	return CstrFmtS(buff, str, stack);
}

u4 CstrLen(u1* str) {
	u4 i = 0;
	while (str[i]) { i++; }
	return i;
}

u1 CstrCmp(u1* str1, u1* str2) {
	while (*str1 == *str2 && *str1) { str1++; str2++; }
	return *str1 == *str2;
}

u1 CstrCmpS(u1* str1, u1* str2, u4 size) {
	for (u4 i = 0; i < size; i++) if (*str1++ != *str2++) return 0;
	return 1;
}

inline u1 CharIsSpace(u1 c){ return c == ' ' || (u1)(c - 9) < 14; }
inline u1 CharIsNL(u1 c){ return c == '\r' || c == '\n'; }
inline u1 CharIsAlp(u1 c){ return (u1)((c & 0xDF) - 'A') < 26; }
inline u1 CharIsNum(u1 c){ return (u1)(c - '0') < 10; }
inline u1 CharIsNumX(u1 c){ return (u1)(c - '0') < 10 || ((u1)((c & 0xDF) - 'A') < 6); }
inline u1 CharIsAlpNum(u1 c){ return (u1)((c & 0xDF) - 'A') < 26 || (u1)((c - '0')) < 10; }
inline u1 CharIsPrintable(u1 c) { return (u1)(c - 32) < 95; }
#endif//  STRWEA_DEF