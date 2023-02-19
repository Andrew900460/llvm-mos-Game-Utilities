#pragma once
#include "fixed16.hpp"

// This will not be considered "official code" and you should probably not use this
// In your projects.
// This is for testing/comparing the performance of sqrt functions for fixed16


// Stand-in code that I got from somwhere and made slight edits to,
// so there was at least a sqrt function available.
// It appears to just use Newton's Method, or some other "guess and adjust" method.
// Also appears that a lot of the slowness is due to needing to do 32 bit division.
// 6502 clock time of ~[7599,12493] (results are ~40% faster with 'x.whole == x_old.whole')
fixed16 sqrt_original() {
		fixed16 n = *this;
		fixed16 x = 0x1000;
		uint32 n_one = (uint32)n.value << 8;
		while(true) {
			fixed16 x_old = x;
			x = (x.value + (uint16)(n_one / x.value)) >> 1;
			if(x.whole == x_old.whole) break;
			// if you replace the above expression with just 'x == x_old'
			// it will go slower, but be a bit more accurate
		}
		return x;
}

// int32_t isqrt(int32_t n) {
//     assert(("sqrt input should be non-negative", n > 0));

//     // Xₙ₊₁
//     int32_t x = n;

//     // cₙ
//     int32_t c = 0;

//     // dₙ which starts at the highest power of four <= n
//     int32_t d = 1 << 30; // The second-to-top bit is set.
//                          // Same as ((unsigned) INT32_MAX + 1) / 2.
//     while (d > n)
//         d >>= 2;

//     // for dₙ … d₀
//     while (d != 0) {
//         if (x >= c + d) {      // if Xₘ₊₁ ≥ Yₘ then aₘ = 2ᵐ
//             x -= c + d;        // Xₘ = Xₘ₊₁ - Yₘ
//             c = (c >> 1) + d;  // cₘ₋₁ = cₘ/2 + dₘ (aₘ is 2ᵐ)
//         }
//         else {
//             c >>= 1;           // cₘ₋₁ = cₘ/2      (aₘ is 0)
//         }
//         d >>= 2;               // dₘ₋₁ = dₘ/4
//     }
//     return c;                  // c₋₁
// }