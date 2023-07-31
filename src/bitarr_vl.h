#ifndef BITARR_VL_
#define BITARR_VL_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "common.h"
#include "bitops.h"

typedef struct {
    size_t k;
    size_t length; // Length of A
    size_t logical_size; // Length of B
    size_t physical_size; // Length of W
    size_t element_size; // Size of each word in W
    uint32_t *W;
    size_t P[];
} VLBitArray;


void VLBitArray_free(VLBitArray *bit_arr);

VLBitArray *VLBitArray_init(
    unsigned int A[], size_t length, size_t k, size_t size
);


uint32_t VLBitArray_read(VLBitArray* bit_arr, size_t i);

#endif // !BITARR_VL_
