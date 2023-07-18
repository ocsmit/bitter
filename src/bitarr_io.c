#include "bitarr_io.h"
#include "bitarr.h"
#include <stdio.h>
#include <string.h>


void BitArray_save(BitArray* bitarr, FILE *fp)
{

    // Write file identifier
    fwrite(&BIT_MAGIC_NUMBER, sizeof(char), strlen(BIT_MAGIC_NUMBER), fp);

    // BitArray metadata needed to construct data structure
    fwrite(&bitarr->l, sizeof(uint8_t), 1, fp);
    fwrite(&bitarr->width, sizeof(uint8_t), 1, fp);
    fwrite(&bitarr->n, sizeof(uint32_t), 1, fp);

    // Write out compressed array
    unsigned int n_entries = 1 + (((bitarr->l * bitarr->n) - 1) / WORD_SIZE);
    fwrite(&(bitarr->v), sizeof(unsigned int), n_entries, fp);
}

BitArray* BitArray_open(FILE *fp)
{
    uint8_t l, width;
    uint32_t n; 
    char magic_number[strlen(BIT_MAGIC_NUMBER)+1];     

    // Verify it is a correct file
    fread(magic_number, sizeof(char), strlen(BIT_MAGIC_NUMBER), fp);
    if (strcmp(magic_number, BIT_MAGIC_NUMBER) != 0) {
        fprintf(stderr, "Incorrect file, magic string does not match\n");
        exit(FILE_ERROR);
    }

    // Read in data to allocate memory on heap
    fread(&l, sizeof(uint8_t), 1, fp);
    fread(&width, sizeof(uint8_t), 1, fp);
    fread(&n, sizeof(uint32_t), 1, fp);
    BitArray* bitarr = BitArray_calloc(n, l);

    // Read in compressed array
    fread(&(bitarr->v), WORD_SIZE, 1, fp);
    return bitarr;
}
