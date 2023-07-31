#ifndef ENCODING_H_
#define ENCODING_H_

#include <stdint.h>
#include <math.h>
#include "bitops.h"




uint32_t count_trailing_zeros(unsigned int v);

unsigned int unary_encode(uint32_t k);

unsigned int gamma_encode(uint32_t k);

unsigned int gamma_decode(unsigned int k);

#endif // !ENCODING_H_

