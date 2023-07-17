#ifndef BITARR_IO_H_
#define BITARR_IO_H_

#include "bitarr.h"


// TODO: Documentation
BitArray* BitArray_open(FILE *fp);
void BitArray_save(BitArray* bitarr, FILE *fp);

#endif // BITARR_IO_H_
