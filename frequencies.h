#ifndef __FREQUENCIES_H_V1__
#define __FREQUENCIES_H_V1__
#include <stdint.h>

typedef unsigned char uchar;
typedef uint64_t Frequencies[256];

bool calc_frequencies(Frequencies freqs, const char* path, const char** a_error);

#endif  // __FREQUENCIES_H__
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
