/**
 * @file
 * @brief Routines for encoding and decoding integers
 */

#include "encoding.h"

#include <stdint.h>
#include <stdio.h>


uint32_t count_trailing_zeros(unsigned int v)
{
    // modified from:
    // http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightMultLookup
    static const uint32_t Mod37BitPosition[] = {
      32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4,
      7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5,
      20, 8, 19, 18
    };

    // map a bit value mod 37 to its position
    return Mod37BitPosition[(-v & v) % 37];
}


unsigned int unary_encode(uint32_t k)
{
    // code 1 . 0 k times (e.g. 3 := 1 . 000
    return (1 << k);

}

unsigned int gamma_encode(uint32_t k)
{
    uint32_t length, offset;

    // floor(log_2(k))
    length = (uint32_t) log2(k);
    offset = k - (1 << length);

    // Unary coded offset . length 
    // e.g. 13 (l = 3, o = 5) := 101.1000
    return (offset << (length+1) | ((1 << length)));
}

unsigned int gamma_decode(unsigned int k)
{
    uint32_t l = 0;
    uint32_t o = 0;

    l = count_trailing_zeros(k);
    k >>= l;

    // Mask for l bits
    o = (1 << l) - 1 & (k >> 1);
    return (1 << l) + o;
}


