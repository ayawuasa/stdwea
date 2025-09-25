#ifndef MATHWEA_INCLUDE
#define MATHWEA_INCLUDE
#include "p:\stdwea\gdefwea.h"

#define PI 3.141592653589793238462643383279502884

inline u4 Round(r4 f);

inline r4 MulAdd(r4 f1, r4 f2, r4 f3);

void SineCosine(r4 x, r4* sc);

u8 Pow(u8 a, u8 b);

inline s8 Abs(s8 i);
inline s8 Max(s8 a, s8 b);
inline s8 Min(s8 a, s8 b);
#endif// MATHWEA_INCLUDE

#ifdef MATHWEA_DEF

inline u4 Round(r4 f) { return (u4)(f + (f < 0 ? -0.5f : 0.5f)); }

inline r4 MulAdd(r4 f1, r4 f2, r4 f3){ return f1 * f2 + f3; }

void SineCosine(r4 x, r4 *sc) {
    u4 notSignBit = 0x7FFFFFFF;
    u4 rangeTiny = 0x39800000;
    union { r4 f; u4 u; } u;

    u.f = x;
    // Small angle approximation, handles signed 0's and denormals, skip this if you don't care.
    u4 i = u.u & notSignBit;  // |x| as s4
    if (i < rangeTiny) {                       // |x| < ~0.000244141
        sc[0] = x;
        sc[1] = 1.0f;
        return;
    }

    // Extended precision modular arithmetic.
    u4 q = Round(x * 2.0f / PI);
    r4 qf = q;
    x = MulAdd(qf, -3.140625f * 0.5f, x);
    x = MulAdd(qf, -0.0009670257568359375f * 0.5f, x);
    x = MulAdd(qf, -6.2771141529083251953e-7f * 0.5f, x);
    x = MulAdd(qf, -1.2154201256553420762e-10 * 0.5f, x);

    // Note: This can be solved faster in the non-vectorized case by
    // computing both polynomials at the same time, when vectorized
    // it is faster to just evaluate them like this.

    r4 y = x * x;

    r4 s = +2.6083159809786593541503e-6f;
    s = MulAdd(s, y, -1.981069071916863322258e-4f);
    s = MulAdd(s, y, +8.33307858556509017944336e-3f);
    s = MulAdd(s, y, -1.66666597127914428710938e-1f);
    s = MulAdd(s, (y * x), x);

    r4 c = -2.718118424e-7f;
    c = MulAdd(c, y, +2.479904470e-5f);
    c = MulAdd(c, y, -1.388887875e-3f);
    c = MulAdd(c, y, +4.166666418e-2f);
    c = MulAdd(c, y, -0.5f);
    c = MulAdd(c, y, +1.0f);

    if (q & 1) { // maybe swap sine and cosine
        r4 swap = c;
        c = s;
        s = swap;
    }
    if (q & 2) { // maybe negate sine
        s = -s;
    }
    if ((q + 1) & 2) { // maybe negate cosine
        c = -c;
    }

    sc[0] = s;
    sc[1] = c;
}

u8 Pow(u8 a, u8 b) {
    u8 r = 1;
    while (b) {
        if (b & 1) { r *= a; }
        a *= a;
        b >>= 1;
    }
    return r;
}

inline s8 Abs(s8 i) { return i < 0 ? -i : i; }
inline s8 Max(s8 a, s8 b) { return a < b ? b : a; }
inline s8 Min(s8 a, s8 b) { return a > b ? b : a; }
#undef MATHWEA_DEF
#endif// MATHWEA_DEF