/**
 * @file
 * @brief Operations for BitArray
 */


#include "bitarr.h"

extern inline unsigned int sig_bit_idx(unsigned int j, unsigned int word_size)
{
  return ((j) % word_size) + 1;
}

unsigned int BitArray_bitread(BitArray* bit_arr, unsigned int j) {
  // idx of bit j in word of size w
  unsigned int r = sig_bit_idx(j, bit_arr->size);

  // TODO: Add range checking
  return (bit_arr->v[j/bit_arr->size] >> (j % bit_arr->size)) & 1;
}


void BitArray_bitset(BitArray* bit_arr, unsigned int j)
{
  bit_arr->v[j/bit_arr->size] |= 1 << (j % bit_arr->size);
}

void BitArray_bitclear(BitArray* bit_arr, unsigned int j)
{
  bit_arr->v[j/bit_arr->size] &= ~(1 << (j % bit_arr->size));
}

int BitArray_bitsread(BitArray* bit_arr, unsigned int j1, unsigned int j)
{
  if (j1 > j) return 0;

  if (j1 / bit_arr->size == j / bit_arr->size) {
    return 
      (bit_arr->v[j/bit_arr->size] >> (j1 % bit_arr->size)) & ((1 << (j-j1+1)) - 1);
  }

  return (
  (bit_arr->v[j1/bit_arr->size] >> (j1 % bit_arr->size)) |
    (bit_arr->v[j/bit_arr->size] & 
    ((1 << ((j+1) % bit_arr->size)) - 1)) <<
    (bit_arr->size - (j1 % bit_arr->size))
);
}

int BitArray_read(BitArray* bit_arr, unsigned int i)
{
  return BitArray_bitsread(bit_arr, i*bit_arr->l, (i+1)*bit_arr->l-1); 
}

