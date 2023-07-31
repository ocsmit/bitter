#ifndef BITOPS_H_
#define BITOPS_H_

#include <stdlib.h>
#include <stdint.h>
#include "common.h"


uint32_t find_LSB(uint32_t v);
uint32_t find_MSB(uint32_t v);

void bit_set(uint32_t *bit_arr, size_t size, size_t j);
void bit_clear(uint32_t *bit_arr, size_t size, size_t j);
void bit_write_range(
  uint32_t *bit_arr, size_t w, unsigned int j1, unsigned int j, unsigned int x
);

unsigned bit_read(uint32_t *bit_arr, size_t size, size_t j);

unsigned int bit_read_range(
  uint32_t *bit_arr, size_t width, unsigned int j1, unsigned int j
);

#endif // !BITOPS_H_
