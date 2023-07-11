#include "tests.h"
#include "assert.h"
#include <stdio.h>
#include "bitarr.h"
#include "bitops.h"



BEGIN_TESTING

// Data
unsigned int A[10] = { 20, 18, 22, 22, 16, 21, 11, 22, 21, 21 };

// == 10100 10010 10110 10110 10000 10101 01011 10110 10101 10101
unsigned int correct_B[50] = {
    1,0,1,0,0, 
    1,0,0,1,0, 
    1,0,1,1,0, 
    1,0,1,1,0, 
    1,0,0,0,0, 
    1,0,1,0,1, 
    0,1,0,1,1, 
    1,0,1,1,0, 
    1,0,1,0,1, 
    1,0,1,0,1,
};

unsigned int correct_W[2] = { 2762827861, 1991065600 };

BitArray* bit_arr = BitArray_calloc(2, 5);

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

TEST("BitArray calloc") {
    assert(bit_arr != NULL);
    for (int i = 0; i<2; ++i) bit_arr->v[i] = correct_W[i];
    for (int i = 0; i<2; ++i) assert(bit_arr->v[i] == correct_W[i]);
    printf("✔ BitArray calloc\n");
}

TEST("single bit read") {
    unsigned int b;
    for (unsigned int i = 0; i < 50; ++i) {
        // if (i % 5 == 0) printf(" ");
        b = BitArray_bitread(bit_arr, i);
        // printf("%d", b);
        assert(b == correct_B[i]);
    }
    // printf("\n");
    printf("✔ bit read passed\n");
}

TEST("bit set & clear") {
    unsigned int og_bit, nu_bit;
    og_bit = BitArray_bitread(bit_arr, 0);
    
    // 1 -> 1
    BitArray_bitset(bit_arr, 0);
    nu_bit = BitArray_bitread(bit_arr, 0);
    assert((og_bit & nu_bit) == 1);

    // 1 -> 0
    BitArray_bitclear(bit_arr, 0);
    nu_bit = BitArray_bitread(bit_arr, 0);
    assert(nu_bit == 0);

    // 0 -> 0
    BitArray_bitclear(bit_arr, 0);
    nu_bit = BitArray_bitread(bit_arr, 0);
    assert(nu_bit == 0);

    // 0 -> 1 (back to original)
    BitArray_bitset(bit_arr, 0);
    nu_bit = BitArray_bitread(bit_arr, 0);
    assert((og_bit & nu_bit) == 1);
    printf("✔ bit set/clear passed\n");
}


TEST("bits read range") {
    int num;

    //A :=  20  18  22  22  16  21  11  22  21  21
    for (unsigned int i = 0; i < 10; ++i) {
        num = BitArray_read(bit_arr, i);
        assert((unsigned) num == A[i]);
        // printf(" %d ", num);
    }
    // printf("\n");
    printf("✔ BitArray read\n");



}


END_TESTING
