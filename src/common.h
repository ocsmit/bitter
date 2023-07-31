#ifndef COMMON_H_
#define COMMON_H_

#define ceil_int(x, y) (1 + (x - 1) / y)

typedef enum {
  BITARR_SUCCESS,
  OUT_OF_BOUNDS,      // Indexing error
  FILE_ERROR          // I/O Error
} BITARR_ERROR;

#endif