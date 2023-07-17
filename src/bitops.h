#ifndef BITOPS_H_
#define BITOPS_H_

#include <stdlib.h>
#include "bitarr.h"


/**
 * @brief Find significant bit of index j in compressed entry
 *
 * @param j 
 * @param word_size 
 * @return bit index
 */
extern inline unsigned int sig_bit_idx(unsigned int j, unsigned int word_size);

/**
 * @brief Read single bit at index `j`
 *
 * @param bit_arr   Pointer to BitArray
 * @param j         Index 
 * @return          bit value
 */
unsigned int BitArray_bitread(BitArray* bit_arr, unsigned int j);

/**
 * @brief Set bit at index `j`
 *
 * Sets the the bit = 1 at index `j`. If bit is already equal to 1 then nothing
 * happens.
 *
 * @param bit_arr   Pointer to BitArray
 * @param j         Index 
 */
void BitArray_bitset(BitArray* bit_arr, unsigned int j);

/**
 * @brief Clears bit at index `j`
 *
 * Sets the the bit = 0 at index `j`. If bit is already equal to 0 then nothing
 * happens.
 *
 * @param bit_arr   Pointer to BitArray
 * @param j         Index 
 */
void BitArray_bitclear(BitArray* bit_arr, unsigned int j);

/**
 * @brief Reads range of bits B[j1, j]
 *
 * @param bit_arr 
 * @param j1 
 * @param j 
 * @return Integer constructed from bits [j1, j] 
 */
int BitArray_bitsread(BitArray* bit_arr, unsigned int j1, unsigned int j);


/**
 * @brief Get value from original array at index i
 *
 * The array held within a BitArray is a compact version of the original.
 * We can retrieve this original value by reading the bits from the range 
 * [i * l, (i+1)*l-1] in the compact array.
 *
 * @param bit_arr 
 * @param i 
 * @return Value at A[i]
 */
int BitArray_read(BitArray* bit_arr, unsigned int i);


/**
 * @brief Write to range of bits
 *
 * Abstract function, most will want to use BitArray_write instead
 *
 * @param bit_arr Pointer to BitArray
 * @param j1      Starting index for virtual bit array
 * @param j       Ending index for virtual bit array
 * @param x       Integer to write
 */
void BitArray_bitswrite(BitArray* bit_arr, unsigned int j1, unsigned int j, unsigned int x);



/**
 * @brief Write value to compact bit representation of array
 *
 * @param bit_arr Pointer to BitArray
 * @param i       Index in array to write
 * @param x       Integer to write
 */
void BitArray_write(BitArray* bit_arr, unsigned int i, unsigned int x);

#endif // !BITOPS_H_
