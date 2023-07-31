#include "tests.h"
#include "assert.h"
#include <stdint.h>
#include <stdio.h>
#include "../src/bitarr.h"
#include "../src/bitops.h"
#include "../src/bitarr_io.h"
#include "../src/encoding.h"
#include "../src/bitarr_vl.h"




BEGIN_TESTING


// -- Data --------------------------------------------------------------------
unsigned int A[10] = { 20, 18, 22, 22, 16, 21, 11, 22, 21, 21 };
unsigned int A_vl[10] = { 0, 1, 0, 2, 5, 1, 3, 2, 8, 2 };

/*
* Both of binary representations of b have been flipped since when reading
* individual bits the most from array A, the least significant bit will be read
* first from each int.
*
* 394338780 := 1,1,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1,0,0,
* 177586    := 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,0,1,0,
*/
unsigned int B_sig_ordered[64] = {
    0,0,1,0,1,0,1,0,0,1,0,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,0,1,0,1,1,1,
    0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
unsigned int correct_W[2] = { 3943389780, 177586} ;
BitArray* bit_arr = BitArray_init(A, (sizeof(A)/sizeof(A[0])), 5, sizeof(uint32_t));

static char bit_arr_fp[] = "./data/bitarr_test.bit";

// ----------------------------------------------------------------------------


TEST("BitArray calloc")
{
    assert(bit_arr != NULL);
    printf("✔ BitArray calloc\n");
}

TEST("bits write")
{
    unsigned int num;
    unsigned int i = 0;

    for (i = 0; i < 10; ++i) {
        num = BitArray_read(bit_arr, i);
        assert(num == A[i]);
    }

    for (i = 0; i < 2; ++i) assert(bit_arr->v[i] == correct_W[i]);
    printf("✔ BitArray write\n");
}


TEST("single bit read")
{
    unsigned int b;
    for (unsigned int i = 0; i < 64; ++i) {
        b = bit_read(bit_arr->v, bit_arr->width, i);
        assert(b == B_sig_ordered[i]);
    }
    printf("✔ bit read passed\n");
}

TEST("bit set & clear")
{
    unsigned int og_bit, nu_bit, idx;
    idx = 2;
    og_bit = bit_read(bit_arr->v, bit_arr->width, idx);

    // 1 -> 1
    bit_set(bit_arr->v, bit_arr->width, idx);
    nu_bit = bit_read(bit_arr->v, bit_arr->width, idx);
    assert((og_bit & nu_bit) == 1);

    // 1 -> 0
    bit_clear(bit_arr->v, bit_arr->width, idx);
    nu_bit = bit_read(bit_arr->v, bit_arr->width, idx);
    assert(nu_bit == 0);

    // 0 -> 0
    bit_clear(bit_arr->v, bit_arr->width, idx);
    nu_bit = bit_read(bit_arr->v, bit_arr->width, idx);
    assert(nu_bit == 0);

    // 0 -> 1 (back to original)
    bit_set(bit_arr->v, bit_arr->width, idx);
    nu_bit = bit_read(bit_arr->v, bit_arr->width, idx);
    assert((og_bit & nu_bit) == 1);
    printf("✔ bit set/clear passed\n");
}


TEST("bits read range")
{
    unsigned int num;

    //A :=  20  18  22  22  16  21  11  22  21  21
    for (unsigned int i = 0; i < 10; ++i) {
        num = BitArray_read(bit_arr, i);
        assert((unsigned) num == A[i]);
    }
    printf("✔ BitArray read\n");

}

TEST("Write to disk")
{
    FILE *fp = fopen(bit_arr_fp, "wb");
    BitArray_save(bit_arr, fp);
    fclose(fp);
    printf("✔ BitArray disk write\n");
}

TEST("Read from disk")
{
    FILE *fp = fopen(bit_arr_fp, "rb");
    BitArray* bit_arr_read = BitArray_open(fp);
    fclose(fp);

    unsigned int num;
    for (unsigned int i = 0; i < 10; ++i) {
        num = BitArray_read(bit_arr_read, i);
        assert((unsigned) num == A[i]);
    }

    BitArray_free(bit_arr_read);
    printf("✔ BitArray disk read\n");
}

BitArray_free(bit_arr);

TEST("Gamma encoding")
{
    uint32_t og_int = 13;
    unsigned int gc = gamma_encode(og_int);
    gc = gamma_decode(gc);
    assert(og_int == gc);
    printf("✔ Gamma coding\n");
}

TEST("VL BitArray")
{


    uint32_t correct_W_vla[2] = { 415519957, 3 };
    VLBitArray *vlb = VLBitArray_init(A_vl, 10, 4, sizeof(uint32_t));
    uint32_t AA[31] = {1, 100, 200, 11, 1, 50, 1000};
    AA[20] = 20;
    AA[30] = 10000;
    VLBitArray *vlb1 = VLBitArray_init(AA, 31, 30, sizeof(uint32_t));

    for (size_t i = 0; i < 2; i++) {
        assert(correct_W_vla[i] == vlb->W[i]);
    }
    for (size_t i = 0; i < 10; ++i) assert(A_vl[i] == VLBitArray_read(vlb, i));

    VLBitArray_free(vlb);

    for (size_t i = 0; i < 31; ++i) {
        assert(AA[i] == VLBitArray_read(vlb1, i));
    }

    printf("✔ Variable Length BitArray\n");
}




TEST("tree") {
    // int matrix[16][16] = {
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,},
    //     {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
    // };
}


END_TESTING
