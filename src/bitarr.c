#include "bitarr.h"
#include "bitops.h"

BitArray* BitArray_calloc(unsigned int n, unsigned int l)
{
    unsigned int n_entries = 1 + (((l * n) - 1) / WORD_SIZE);
    // space for bitarray + space needed for n_entries of word_size
    BitArray *bitarr = calloc(1, sizeof(BitArray) + WORD_SIZE * n_entries);
    if (bitarr == NULL) {
        printf("Couldn't allocate memory for vector.\n");
        exit(EXIT_FAILURE);
    }
    // Set values
    bitarr->l = l;
    bitarr->width = WORD_SIZE;
    bitarr->n = n;
    
    return bitarr;
}

void BitArray_free(BitArray *bitarr) {
    free(bitarr);
}


BitArray* BitArray_init(unsigned int A[], unsigned int n, unsigned int l)
{
    BitArray* bit_arr = BitArray_calloc(n, l);
    // Compress values from A into BitArray
    unsigned int i;
    for (i = 0; i < n; ++i) BitArray_write(bit_arr, i, A[i]);
    return bit_arr;
}
