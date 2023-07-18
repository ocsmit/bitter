# bitter
Compact/succinct representations for vectors and arrays

## BitArray

Compact bit arrays are space effecient representation of the classical array
as described in "[Compact Data Structures, a practical approach](https://doi.org/10.1017/CBO9781316588284)" .

Arrays are stored in a compact form by using only the bits needed for each
element. E.g. below the array A is shown to be stored using only two uint32
elements (W). From the compact representation, the values from the original
array (A) can be read, written, and operated on.
 
```
     ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
  A  │  20  │  18  │  22  │  22  │  16  │  21  │  11  │  22  │  21  │  21  │
     └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
         │      │      │      │      │      │      │      │      │      │
         │      │      │      │      │      │      │      │      │      │
         ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼
     ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
  B  │10100 │10010 │10110 │10110 │10000 │10101 │01011 │10110 │10101 │10101 │ 
     └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
         │      │      │      │      │      │      │      │      │      │
         │      │      │  B is a virtual bit array │      │      │      │
         │      │      │      │      │      │      │      │      │      │
         ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼      ▼
     ┌─────────────────────────────────────────────────────────────────────┐
  B  │10100  10010  10110  10110  10000  10101  01011  10110  10101  10101 │
     └─────────────────────────────────────────────────────────────────────┘
  │                                              ┌────────────────────────┐
  │   The bits are reordered to provide          │                        │
  │   0 based indexing from W                 ┌─────┬─────┐               │
  │                                       MSB │ 010 │ 11  │ LSB           │
  │                                           └─────┴─────┘               │
  │                                               A[6] │                  │
  │    ┌───────────────────────────────────────────────┘                  │
  ▼  ┌ ┼ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┬ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─│─ ┐
       ▼                                                                  ▼
  W  │11  10101  10000  10110  10110  10010  10100 │10101  10101  10110  010 │
      ^^                                                                 ^^^
     └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┴ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┘
      A[6]                  │                 A[0]   A[9]       │        A[6]
            ┌───────────────┘                                   │
            │            ┌──────────────────────────────────────┘
            ▼            ▼
     ┌────────────┬────────────┐
  W  │ 3943389780 │   177586   │
     └────────────┴────────────┘
``` 
It is key to note that the values of A are stored lowest significant first.
The same is true for entries which span multiple words. E.g. A[6] in the
above visual is split with the 2 lowest sig bits in word 1, and the three
highest sig bits in word 2. In word 1 the lowest sig bits of A[6] will be
the last bits accessed, while in word 2 the highest sig bits of A[6] will
be accessed first.

for implementation and operation details see:
  - [bitarr.h](src/bitarr.h)
  - [bitops.h](src/bitops.h)
  - [bitarr_io.h](src/bitarr_io.h)

