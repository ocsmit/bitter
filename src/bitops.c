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
  return (bit_arr->v[j/bit_arr->size] / (1 << (bit_arr->size - r))) % 2;
}


void BitArray_bitset(BitArray* bit_arr, unsigned int j)
{
  // idx of bit j in word of size w
  unsigned int r, idx, word;
  r = sig_bit_idx(j, bit_arr->size); 
  idx = j/bit_arr->size;
  word = bit_arr->v[idx];


  if ((word / (1 << (bit_arr->size - r))) % 2 == 0) {
    bit_arr->v[idx] = word + (1 << (bit_arr->size - r));
  }
}

void BitArray_bitclear(BitArray* bit_arr, unsigned int j)
{

  // idx of bit j in word of size w
  unsigned int r, idx, word;
  r = sig_bit_idx(j, bit_arr->size); 
  idx = j/bit_arr->size;
  word = bit_arr->v[idx];

  if ((word / (1 << (bit_arr->size - r))) % 2 == 1) {
    bit_arr->v[idx] = word + (1 << (bit_arr->size - r));
  }
}

int BitArray_bitsread(BitArray* bit_arr, unsigned int j1, unsigned int j)
{
  if (j1 > j) return 0;

  // idx of j
  unsigned int r = sig_bit_idx(j, bit_arr->size);

  // Early return if bits contained in same element in W
  if (j1 / bit_arr->size == j / bit_arr->size) {
    return (int)(
      (bit_arr->v[j/bit_arr->size] / (1 << (bit_arr->size - r))) % (1 << (j - j1 + 1))
    );
  }

  // idx of j'
  unsigned int r1= sig_bit_idx(j1, bit_arr->size) - 1;
  return (int)(
    (bit_arr->v[j/bit_arr->size] / (1 << (bit_arr->size - r))) +
    (bit_arr->v[j1/bit_arr->size] % (1 << (bit_arr->size - r1))) *
    (1 << r)
  );
}

int BitArray_read(BitArray* bit_arr, unsigned int i)
{
  return BitArray_bitsread(bit_arr, i*bit_arr->l, (i+1)*bit_arr->l-1); 
}

