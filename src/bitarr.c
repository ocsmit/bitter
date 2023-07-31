#include "bitarr.h"
#include "bitops.h"
#include <stdint.h>

BitArray* BitArray_calloc(uint32_t n, uint8_t element_size, size_t word_size)
{
    unsigned int n_entries = 1 + (((element_size * n) - 1) / 32);
    // space for bitarray + space needed for n_entries of word_size
    BitArray *bitarr = calloc(1, sizeof(BitArray) + (word_size * CHAR_BIT) * n_entries);
    if (bitarr == NULL) {
        printf("Couldn't allocate memory for vector.\n");
        exit(EXIT_FAILURE);
    }
    // Set values
    bitarr->element_size = element_size;
    bitarr->width = word_size * CHAR_BIT;
    bitarr->n = n;

    return bitarr;
}

void BitArray_free(BitArray *bitarr) {
    free(bitarr);
}

BitArray* BitArray_init(unsigned int A[], uint32_t n, uint8_t element_size, size_t word_size)
{
    BitArray* bit_arr = BitArray_calloc(n, element_size, word_size);
    // Compress values from A into BitArray
    unsigned int i;
    for (i = 0; i < n; ++i) BitArray_write(bit_arr, i, A[i]);
    return bit_arr;
}


unsigned int BitArray_read(BitArray* bit_arr, unsigned int i)
{
  if (i >= bit_arr->n) {
    fprintf(stderr, "%s:%d Out of bounds index\n", __FILE__, __LINE__);
    exit(OUT_OF_BOUNDS);
  }
  return bit_read_range(bit_arr->v, bit_arr->width, i*bit_arr->element_size,
    (i+1)*bit_arr->element_size-1);
}

// -- Writing -----------------------------------------------------------------
void BitArray_write(BitArray* bit_arr, unsigned int i, unsigned int x)
{
  if (i >= bit_arr->n) {
    fprintf(stderr, "%s:%d Out of bounds index\n", __FILE__, __LINE__);
    exit(OUT_OF_BOUNDS);
  }
  bit_write_range(bit_arr->v, bit_arr->width, i*bit_arr->element_size,
    (i+1)*bit_arr->element_size-1, x);
}


