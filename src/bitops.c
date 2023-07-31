/**
 * @file
 * @brief Operations for BitArray
 */

#include "bitops.h"
#include <stdint.h>


uint32_t find_LSB(uint32_t v)
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


uint32_t find_MSB(uint32_t v)
{

    static const uint32_t MultiplyDeBruijnBitPosition[32] =
        {
            0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
            8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
        };

    v |= v >> 1; // first round down to one less than a power of 2
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    return MultiplyDeBruijnBitPosition[(uint32_t)(v * 0x07C4ACDDU) >> 27];
}


// -- Writing -----------------------------------------------------------------
void bit_set(uint32_t *bit_arr, size_t size, size_t j)
{
  // Shift word left to bit idx, OR w/ 1
  bit_arr[j/size] |= 1 << (j % size);
}


void bit_clear(uint32_t* bit_arr, size_t size, size_t j)
{
  // Shift word left to bit idx, AND w/ NOT(1)
  bit_arr[j/size] &= ~(1 << (j % size));
}


void bit_write_range(
  uint32_t *bit_arr, size_t w, unsigned int j1, unsigned int j, unsigned int x
)
{
  if (j1 > j) return; // Early return if start idx > end idx

  // Confined w/in single word
  if (j1 / w == j / w) {
    // Clear bits
    bit_arr[j/w] &= (
      ~((unsigned) ((1 << (j-j1+1)) - 1) << (j1 % w))
    );
    // Write x bits
    bit_arr[j/w] |= x << (j1 % w);
  } else {
    // Spans two words
    bit_arr[j1/w] = (
      // Get bits in first word to store lower bits
      (bit_arr[j1/w] & ((1 << (j1 % w)) - 1)) |
      // Write bits
      (x <<  (j1 % w))
    );

    bit_arr[j/w] = (
      // Get bits in second word to store lower bits
      (bit_arr[j/w] & ~((1 << ((j+1) % w)) - 1)) |
      // Write bits
      (x >> (w - (j1 % w)))
    );
  }
}


// -- Reading -----------------------------------------------------------------
unsigned bit_read(uint32_t *bit_arr, size_t size, size_t j)
{
  return (bit_arr[j/size] >> (j % size)) & 1;
}

unsigned int bit_read_range(
  uint32_t *bit_arr, size_t width, unsigned int j1, unsigned int j
)
{
  if (j1 > j) return 0; // Early return if start idx > end idx

  // Confined w/in single word
  if (j1 / width == j / width) {
    return (
      // Shift word right
      (bit_arr[j/width] >> (j1 % width)) &
      // AND on bit vector of 1s the necessary length to extract only needed
      // bits
      ((1 << (j-j1+1)) - 1)
    );
  }

  // Spans two words
  return (
  // Get bits in the first word
  (bit_arr[j1/width] >> (j1 % width)) |
  // Bits in second word
  (bit_arr[j/width] &  ((1 << ((j+1) % width)) - 1)) <<
    // Shift bits from second word n bits from first word left to make
    // room for concatenation
    (width - (j1 % width))
  );
}

