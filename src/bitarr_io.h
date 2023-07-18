#ifndef BITARR_IO_H_
#define BITARR_IO_H_

#include <string.h>
#include "bitarr.h"

static const char BIT_MAGIC_NUMBER[] = { 'B', 'I', 'T' };

/**
 * @brief Save BitArray to disk
 *
 * @param bitarr    Pointer to bitarr
 * @param fp        Filepath to save to
 */
void BitArray_save(BitArray* bitarr, FILE *fp);


/**
 * @brief Opens bitarr saved to disk
 *
 * @param fp    Filepath to bitarr saved on disk
 * @return
 */
BitArray* BitArray_open(FILE *fp);

#endif // BITARR_IO_H_
