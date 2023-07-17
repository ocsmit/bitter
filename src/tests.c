#include "tests.h"
#include "assert.h"
#include <stdio.h>
#include "bitarr.h"
#include "bitops.h"



BEGIN_TESTING

// Data
unsigned int A[10] = { 20, 18, 22, 22, 16, 21, 11, 22, 21, 21 };

/*
* Both of binary representations of b have been flipped since when reading individual bits the most from array A, the least significant bit will be read first from each int.
*
* 394338780 := 1,1,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1,0,0,
* 177586    := 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,0,1,0,
*/
unsigned int B_sig_ordered[64] = {
    0,0,1,0,1,0,1,0,0,1,0,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,0,1,0,1,1,1,
    0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

unsigned int correct_W[2] = { 3943389760, 177586} ;


BitArray* bit_arr = BitArray_calloc(2, 5);


TEST("BitArray calloc") {
    assert(bit_arr != NULL);
    printf("✔ BitArray calloc\n");
}

TEST("bits write") {
    unsigned int num, og;
    for (int i = 0; i < 10; ++i) BitArray_write(bit_arr, i, A[i]);
    for (int i = 0; i < 10; ++i) {
        num = BitArray_read(bit_arr, i);
        assert(num == A[i]);
    }

    printf("✔ BitArray write\n");
}


TEST("single bit read") {
    unsigned int b;
    for (unsigned int i = 0; i < 64; ++i) {
        b = BitArray_bitread(bit_arr, i);
    }
    printf("✔ bit read passed\n");
}

TEST("bit set & clear") {
    unsigned int og_bit, nu_bit, idx;
    idx = 2;
    og_bit = BitArray_bitread(bit_arr, 2);
    
    // 1 -> 1
    BitArray_bitset(bit_arr, idx);
    nu_bit = BitArray_bitread(bit_arr, idx);
    assert((og_bit & nu_bit) == 1);

    // 1 -> 0
    BitArray_bitclear(bit_arr, idx);
    nu_bit = BitArray_bitread(bit_arr, idx);
    assert(nu_bit == 0);

    // 0 -> 0
    BitArray_bitclear(bit_arr, idx);
    nu_bit = BitArray_bitread(bit_arr, idx);
    assert(nu_bit == 0);

    // 0 -> 1 (back to original)
    BitArray_bitset(bit_arr, idx);
    nu_bit = BitArray_bitread(bit_arr, idx);
    assert((og_bit & nu_bit) == 1);
    printf("✔ bit set/clear passed\n");
}


TEST("bits read range") {
    unsigned int num;

    //A :=  20  18  22  22  16  21  11  22  21  21
    for (unsigned int i = 0; i < 10; ++i) {
        num = BitArray_read(bit_arr, i);
        assert((unsigned) num == A[i]);
    }
    printf("✔ BitArray read\n");

}


BitArray_free(bit_arr);



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
