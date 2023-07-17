#include "bitarr.h"
#include <stdlib.h>
#include <stdio.h>
#include "bitops.h"

BitArray* BitArray_calloc(unsigned int n_entries, unsigned int l)
{
    // Allocate BitArray + array on heap
    BitArray *bitarr = calloc(1, sizeof(BitArray) + l * n_entries);
    if (bitarr == NULL) {
        printf("Couldn't allocate memory for vector.\n");
        exit(EXIT_FAILURE);
    }
    // Set values
    bitarr->l = l;
    bitarr->size = WORD_SIZE;
    
    return bitarr;
}

void BitArray_free(BitArray *bitarr) {
    free(bitarr);
}


BitArray* BitArray_init(unsigned int A[], size_t length, unsigned int l)
{
    // Ceiling division
    unsigned int n_entries = (l * length)/WORD_SIZE + ((l*length) % WORD_SIZE != 0);
    BitArray* bit_arr = BitArray_calloc(n_entries, l);

    // Compress values from A into BitArray
    unsigned int i;
    for (i = 0; i < length; ++i) BitArray_write(bit_arr, i, A[i]);
    return bit_arr;
}
