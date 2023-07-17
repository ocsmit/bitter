/**
 * @file
 * @brief Compact array representation
 *
 * Compact bit arrays are space effecient representation of the classical array
 * as described in "Compact Data Structures, a practical approach" (https://doi.org/10.1017/CBO9781316588284).
 *
 * Arrays are stored in a compact form by using only the bites needed for each
 * element. E.g. below the array A is shown to be stored using only two uint32
 * elements (W).
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
 *         │      │      │      │      │      │      │      │      │      │
 *         ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼
 *     ┌─────────────────────────────────────────────────────────────────────┐
 *  B  │10100  10010  10110  10110  10000  10101  01011  10110  10101  10101 │
 *     └─────────────────────────────────────────────────────────────────────┘
 *  │                                              ┌────────────────────────┐
 *  │                                              │                        │
 *  │                                           ┌─────┬─────┐               │
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
 *            │            │
 *            ▼            ▼
 *     ┌────────────┬────────────┐
 *  W  │ 3943389780 │   177586   │
 *     └────────────┴────────────┘
*/


#ifndef BITARR_H_
#define BITARR_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
 * @var BitArray.size
 *  size of each member in v
 * @var BitArray.v[]
 *  compressed version of A (v)
 *
 */
typedef struct {
  unsigned int l;       // number of bits to store each value in A
  unsigned int size;    // size of each member in v
  unsigned int v[];   // compressed version of A (v)
} BitArray;


/**
 * @brief Allocate empty BitArray with values set to 0 on the heap
 *
 * @param n_entries     Number of words to store
 * @param l             Size in bits of each item
 * @return              Pointer to BitArray
 */
BitArray* BitArray_calloc(unsigned int n_entries, unsigned int l);

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
 * @param length    Length of array A
 * @param l         Maximum number of bits for each element in A
 * @return          pointer to BitArray
 */
BitArray* BitArray_init(unsigned int A[], unsigned int length, unsigned int l);

#endif // BITARR_H_
