#include "bitarr_vl.h"
#include "bitops.h"
#include "encoding.h"

void VLBitArray_free(VLBitArray *bit_arr)
{
    free(bit_arr->W);
    free(bit_arr);
}

VLBitArray *VLBitArray_init(unsigned int A[], size_t length, size_t k, size_t size)
{

    // bytes -> bits
    size_t size_bits = size * 8;

    // Find length of P
    int p_len = ceil_int(length, k);
    // Allocate struct and pointer vla
    VLBitArray *vlb = calloc(1, sizeof(VLBitArray) + sizeof(size_t) * p_len);

    size_t current_p_pos = 0;
    // Create array of size the length of A
    uint32_t gamma_A = 0,
             g_length = 0,
             g_offset = 0,
             A_copy[length]; // Empty array of same size

    for (size_t i = 0, j = 0; i < length; ++i) {
        // Encode value as A[i] + 1 (gamma encoding can't be zero)
        // code will be (g_offset << (g_length+1) | ((1 << g_length)))
        g_length = (uint32_t) log2(A[i] + 1);
        g_offset = (A[i] + 1) - (1 << g_length);

        // Gamma code is of size length * 2
        size_t p_increment = g_length * 2 + 1;

        // Write gamma code of A[i] to array
        bit_write_range(
            A_copy,
            size_bits,
            current_p_pos,
            current_p_pos + p_increment,
            // Encode as g_offset.g_length
            (g_offset << (g_length+1) | ((1 << g_length)))
        );

        // Assign current bit idx to pointer array
        if (i % k == 0) vlb->P[j++] = current_p_pos;
        current_p_pos += p_increment;
    }

    // Maximum number of elements of word size we need to fit total number of bits
    size_t max_idx = ceil_int(current_p_pos, size_bits);
    // Allocate array, and copy over only the needed bits from A_copy
    vlb->W = malloc((size) * max_idx);
    memcpy(vlb->W, A_copy, (size) * max_idx);

    // Set struct members
    vlb->k = k;
    vlb->length = length;
    vlb->logical_size = current_p_pos;
    vlb->physical_size = max_idx;
    vlb->element_size = size_bits;

    return vlb;
}


uint32_t VLBitArray_read(VLBitArray* bit_arr, size_t i)
{
    if (i >= bit_arr->length) {
        fprintf(stderr, "%s:%d Out of bounds index\n", __FILE__, __LINE__);
        exit(OUT_OF_BOUNDS);
    }


    size_t curr_idx = bit_arr->P[ceil_int(i+1, bit_arr->k)-1],
           idx_diff = i - (ceil_int(i+1, bit_arr->k)-1) * bit_arr->k,
           max_idx = 0;

    uint32_t g_length = 0,
             chunk = 0;

    int ii = 0;
    while (ii <= idx_diff) {
        max_idx = curr_idx + bit_arr->element_size - 1;
        chunk = bit_read_range(
            bit_arr->W, 
            bit_arr->element_size, 
            curr_idx,
            max_idx + 
            ((max_idx > bit_arr->logical_size) * (bit_arr->logical_size - max_idx)) - 1
            );

        curr_idx += 1 + ((find_LSB(chunk)) * 2);
        ii++;
    }

    return (gamma_decode(chunk) - 1);
}

