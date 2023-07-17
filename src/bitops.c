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
  // TODO: Add range checking
  return (bit_arr->v[j/bit_arr->size] >> (j % bit_arr->size)) & 1;
}

void BitArray_bitset(BitArray* bit_arr, unsigned int j)
{
  // Shift word left to bit idx, OR w/ 1
  bit_arr->v[j/bit_arr->size] |= 1 << (j % bit_arr->size);
}

void BitArray_bitclear(BitArray* bit_arr, unsigned int j)
{
  // Shift word left to bit idx, AND w/ NOT(1)
  bit_arr->v[j/bit_arr->size] &= ~(1 << (j % bit_arr->size));
}

unsigned int BitArray_bitsread(BitArray* bit_arr, unsigned int j1, unsigned int j)
{
  if (j1 > j) return 0; // Early return if start idx > end idx

  // Confined w/in single word
  if (j1 / bit_arr->size == j / bit_arr->size) { 
    return ( 
      // Shift word right
      (bit_arr->v[j/bit_arr->size] >> (j1 % bit_arr->size)) & 
      // AND on bit vector of 1s the necessary length to extract only needed 
      // bits 
      ((1 << (j-j1+1)) - 1)
    );
  }

  // Spans two words
  return (
  // Get bits in the first word
  (bit_arr->v[j1/bit_arr->size] >> (j1 % bit_arr->size)) | 
  // Bits in second word
  (bit_arr->v[j/bit_arr->size] &  ((1 << ((j+1) % bit_arr->size)) - 1)) <<
    // Shift bits from second word n bits from first word left to make 
    // room for concatenation
    (bit_arr->size - (j1 % bit_arr->size))
  );
}

unsigned int BitArray_read(BitArray* bit_arr, unsigned int i)
{
  return BitArray_bitsread(bit_arr, i*bit_arr->l, (i+1)*bit_arr->l-1); 
}


void BitArray_bitswrite(BitArray* bit_arr, unsigned int j1, unsigned int j, unsigned int x)
{
  if (j1 > j) return; // Early return if start idx > end idx
  unsigned int w = bit_arr->size;

  // Confined w/in single word
  if (j1 / bit_arr->size == j / bit_arr->size) { 
    // Clear bits
    bit_arr->v[j/bit_arr->size] &= ~(((1 << (j-j1+1)) - 1) << (j1 % bit_arr->size));
    // Write x bits
    bit_arr->v[j/bit_arr->size] |= x << (j1 % bit_arr->size); 
  } else {
    // Spans two words
    bit_arr->v[j1/bit_arr->size] = (
      // Get bits in first word to store lower bits
      (bit_arr->v[j1/bit_arr->size] & ((1 << (j1 % bit_arr->size)) - 1)) | 
      // Write bits 
      (x <<  (j1 % bit_arr->size))
    );
    //
    bit_arr->v[j/bit_arr->size] = (
      // Get bits in second word to store lower bits
      (bit_arr->v[j/bit_arr->size] & ~((1 << ((j+1) % w)) - 1)) | 
      // Write bits 
      (x >> (w - (j1 % w)))
    );
  }
}

void BitArray_write(BitArray* bit_arr, unsigned int i, unsigned int x)
{
  BitArray_bitswrite(bit_arr, i*bit_arr->l, (i+1)*bit_arr->l-1, x); 
}
