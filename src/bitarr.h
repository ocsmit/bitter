/**
 * @file
 * @brief Compact array representation
 *
 * Compact bit arrays are space effecient representation of the classical array
 * as described in "Compact Data Structures, a practical approach" (https://doi.org/10.1017/CBO9781316588284).
 *
 * Arrays are stored in a compact form by using only the bits needed for each
 * element. E.g. below the array A is shown to be stored using only two uint32
 * elements (W). From the compact representation, the values from the original
 * array (A) can be read, written, and operated on.
 *
 *     ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 *  A  │  20  │  18  │  22  │  22  │  16  │  21  │  11  │  22  │  21  │  21  │
 *     └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
 *         │      │      │      │      │      │      │      │      │      │
 *         │      │      │      │      │      │      │      │      │      │
 *         ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼
 *     ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
 *  B  │10100 │10010 │10110 │10110 │10000 │10101 │01011 │10110 │10101 │10101 │ 
 *     └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
 *         │      │      │      │      │      │      │      │      │      │
 *         │      │      │  B is a virtual bit array │      │      │      │
 *         │      │      │      │      │      │      │      │      │      │
 *         ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼
 *     ┌─────────────────────────────────────────────────────────────────────┐
 *  B  │10100  10010  10110  10110  10000  10101  01011  10110  10101  10101 │
 *     └─────────────────────────────────────────────────────────────────────┘
 *  │                                              ┌────────────────────────┐
 *  │   The bits are reordered to provide          │                        │
 *  │   0 based indexing from W                 ┌─────┬─────┐               │
 *  │                                       MSB │ 010 │ 11  │ LSB           │
 *  │                                           └─────┴─────┘               │
 *  │                                               A[6] │                  │
 *  │    ┌───────────────────────────────────────────────┘                  │
 *  ▼  ┌ ┼ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┬ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─│─ ┐
 *       ▼                                                                  ▼
 *  W  │11  10101  10000  10110  10110  10010  10100 │10101  10101  10110  010 │
 *      ^^                                                                 ^^^
 *     └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┴ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┘
 *      A[6]                  │                 A[0]   A[9]       │        A[6]
 *            ┌───────────────┘                                   │
 *            │            ┌──────────────────────────────────────┘
 *            ▼            ▼
 *     ┌────────────┬────────────┐
 *  W  │ 3943389780 │   177586   │
 *     └────────────┴────────────┘
 *
 *
 *  It is key to note that the values of A are stored lowest significant first.
 *  The same is true for entries which span multiple words. E.g. A[6] in the
 *  above visual is split with the 2 lowest sig bits in word 1, and the three
 *  highest sig bits in word 2. In word 1 the lowest sig bits of A[6] will be
 *  the last bits accessed, while in word 2 the highest sig bits of A[6] will
 *  be accessed first.
*/


#ifndef BITARR_H_
#define BITARR_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


typedef enum {
  BITARR_SUCCESS,
  OUT_OF_BOUNDS
} BITARR_ERROR;


// The size of each 'word' in W is 4 bytes (32 bits)
// sizeof(uint32_t) == sizeof(int) == (sizeof(unsigned int) * 2)
//
// NOTE: 
// What is the valid type (or size) to consider as the systems word size?
// Is it appropriate to just define the size in our implementation?
//  - https://stackoverflow.com/q/35843365
//  - https://stackoverflow.com/q/14792068
//  
//
#define WORD_SIZE (sizeof(uint32_t) * 8)

/**
 * @struct BitArray
 *
 * Compact BitArray implementation where each element in the original array (A)
 * is the same size.
 *
 * @var BitArray.l 
 *  number of bits to store each value in A
 * @var BitArray.width
 *  number of bits of each member in v (e.g. 32)
 * @var BitArray.n
 *  Length of original array
 * @var BitArray.v[]
 *  compressed version of A (v)
 */
typedef struct {
  uint8_t l;       
  uint8_t width;   
  uint32_t n;
  unsigned int v[]; 
} BitArray;


/**
 * @brief Allocate empty BitArray with values set to 0 on the heap
 *
 * @param n_entries     Number of words to store
 * @param l             Size in bits of each item
 * @return              Pointer to BitArray
 */
BitArray* BitArray_calloc(uint32_t n, uint8_t l);

/**
 * @brief Free BitArray allocated on the heap
 *
 * @param bitarr 
 */
void BitArray_free(BitArray *bitarr);

/**
 * @brief Initialize and populate a BitArray on the heap from an array
 *
 * @param A         1d array
 * @param n         Number of elements in A
 * @param l         Maximum number of bits for each element in A
 * @return          pointer to BitArray
 */
BitArray* BitArray_init(unsigned int A[], uint32_t length, uint8_t l);


#endif // BITARR_H_
