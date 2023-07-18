/*
 * File format is specified as follows
 *  The header is 9 bytes long
 *  - magic number: 3 char   (3 byte)
 *  - l:            1 uint8  (1 byte)
 *  - width:        1 uint8  (1 byte)
 *  - n:            1 uint32 (4 byte)
 *
 * The magic number is specified as "BIT"
 *
 * The length (i) of the data array is computed by ceil((l*n)/32),
 * where 32 is the size of a computer word. Thus the data is read
 * as the next unsigned int * i bytes
 *
 * ┌─────────┬───┬───┬────────────┬───────────────── 
 * │   BIT   │ l │ w │     n      │       ...        
 * └─────────┴───┴───┴────────────┴───────────────── 
 *
 */


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
