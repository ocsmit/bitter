/**
 * @file
 * @brief Routines for encoding and decoding integers
 */

#include "encoding.h"

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

    l = find_LSB(k); // idx of LSB set to 1
    k >>= l;

    // Mask for l bits
    o = (1 << l) - 1 & (k >> 1);
    return (1 << l) + o;
}


