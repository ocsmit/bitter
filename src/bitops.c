/**
 * @file
 * @brief Operations for BitArray
 */


#include "bitarr.h"

extern inline unsigned int sig_bit_idx(unsigned int j, unsigned int word_size)
{
  return ((j) % word_size) + 1;
}

// -- Single bit ops ----------------------------------------------------------

unsigned int BitArray_bitread(BitArray* bit_arr, unsigned int j) {
  return (bit_arr->v[j/bit_arr->width] >> (j % bit_arr->width)) & 1;
}

  
void BitArray_bitset(BitArray* bit_arr, unsigned int j)
{
  // Shift word left to bit idx, OR w/ 1
  bit_arr->v[j/bit_arr->width] |= 1 << (j % bit_arr->width);
}

void BitArray_bitclear(BitArray* bit_arr, unsigned int j)
{
  // Shift word left to bit idx, AND w/ NOT(1)
  bit_arr->v[j/bit_arr->width] &= ~(1 << (j % bit_arr->width));
}


// -- Reading -----------------------------------------------------------------

unsigned int BitArray_bitsread(
  BitArray* bit_arr, unsigned int j1, unsigned int j
)
{
  if (j1 > j) return 0; // Early return if start idx > end idx

  // Confined w/in single word
  if (j1 / bit_arr->width == j / bit_arr->width) { 
    return ( 
      // Shift word right
      (bit_arr->v[j/bit_arr->width] >> (j1 % bit_arr->width)) & 
      // AND on bit vector of 1s the necessary length to extract only needed 
      // bits 
      ((1 << (j-j1+1)) - 1)
    );
  }

  // Spans two words
  return (
  // Get bits in the first word
  (bit_arr->v[j1/bit_arr->width] >> (j1 % bit_arr->width)) | 
  // Bits in second word
  (bit_arr->v[j/bit_arr->width] &  ((1 << ((j+1) % bit_arr->width)) - 1)) <<
    // Shift bits from second word n bits from first word left to make 
    // room for concatenation
    (bit_arr->width - (j1 % bit_arr->width))
  );
}


unsigned int BitArray_read(BitArray* bit_arr, unsigned int i)
{
  if (i >= bit_arr->n) {
    fprintf(stderr, "%s:%d Out of bounds index\n", __FILE__, __LINE__);
    exit(OUT_OF_BOUNDS);
  }
  return BitArray_bitsread(bit_arr, i*bit_arr->element_size,
    (i+1)*bit_arr->element_size-1); 
}

// -- Writing -----------------------------------------------------------------

void BitArray_bitswrite(
  BitArray* bit_arr, unsigned int j1, unsigned int j, unsigned int x
)
{
  if (j1 > j) return; // Early return if start idx > end idx
  unsigned int w = bit_arr->width;

  // Confined w/in single word
  if (j1 / bit_arr->width == j / bit_arr->width) { 
    // Clear bits
    bit_arr->v[j/bit_arr->width] &= (
      ~((unsigned) ((1 << (j-j1+1)) - 1) << (j1 % bit_arr->width))
    );
    // Write x bits
    bit_arr->v[j/bit_arr->width] |= x << (j1 % bit_arr->width); 
  } else {  
    // Spans two words
    bit_arr->v[j1/bit_arr->width] = (
      // Get bits in first word to store lower bits
      (bit_arr->v[j1/bit_arr->width] & ((1 << (j1 % bit_arr->width)) - 1)) | 
      // Write bits 
      (x <<  (j1 % bit_arr->width))
    );
    //
    bit_arr->v[j/bit_arr->width] = (
      // Get bits in second word to store lower bits
      (bit_arr->v[j/bit_arr->width] & ~((1 << ((j+1) % w)) - 1)) | 
      // Write bits 
      (x >> (w - (j1 % w)))
    );
  }
}


void BitArray_write(BitArray* bit_arr, unsigned int i, unsigned int x)
{
  if (i >= bit_arr->n) {
    fprintf(stderr, "%s:%d Out of bounds index\n", __FILE__, __LINE__);
    exit(OUT_OF_BOUNDS);
  }
  BitArray_bitswrite(bit_arr, i*bit_arr->element_size, 
    (i+1)*bit_arr->element_size-1, x); 
}
