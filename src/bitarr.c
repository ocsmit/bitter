#include "bitarr.h"
#include <stdlib.h>
#include <stdio.h>

BitArray* BitArray_calloc(unsigned int n_entries, unsigned int l)
{
    BitArray *bitarr = calloc(sizeof(BitArray),  l * n_entries);
    if (bitarr == NULL) {
        printf("Couldn't allocate memory for vector.\n");
        exit(EXIT_FAILURE);
    }
    bitarr->l = l;
    bitarr->size = WORD_SIZE;
    
    return bitarr;
}

void BitArray_free(BitArray *bitarr) {
    free(bitarr);
}


