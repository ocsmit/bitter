#include "bitarr_io.h"
#include "bitarr.h"
#include <stdio.h>
#include <stdint.h>


// TODO: Documentation
void BitArray_save(BitArray* bitarr, FILE *fp)
{
    fwrite(&bitarr->l, sizeof(uint8_t), 1, fp);
    fwrite(&bitarr->width, sizeof(uint8_t), 1, fp);
    fwrite(&bitarr->n, sizeof(uint32_t), 1, fp);

    unsigned int n_entries = 1 + (((bitarr->l * bitarr->n) - 1) / WORD_SIZE);
    printf("%u\n", n_entries);
    for (int i = 0; i < n_entries; ++i) {
        printf("%u\n", bitarr->v[i]);
        fwrite(&(bitarr->v[i]), sizeof(unsigned int), 1, fp);
    }
}

BitArray* BitArray_open(FILE *fp)
{
    uint8_t l, width;
    uint32_t n; 
    
    fread(&l, sizeof(uint8_t), 1, fp);
    fread(&width, sizeof(uint8_t), 1, fp);
    fread(&n, sizeof(uint32_t), 1, fp);

    BitArray* bitarr = BitArray_calloc(n, l);
    fread(&(bitarr->v), WORD_SIZE, 1, fp);
    return bitarr;
}
