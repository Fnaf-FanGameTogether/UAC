#ifndef _LENGTH_H_
#define _LENGTH_H_

#include <stdint.h>

#ifdef __USE32BIT_LENGTH

typedef uint32_t length_t;

uint8_t _is32bit_enabled(){ return 1; }

#else

typedef uint64_t length_t;

uint8_t _is32bit_enabled(){ return 0; }

#endif





#endif