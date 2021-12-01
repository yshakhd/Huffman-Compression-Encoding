#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include "frequencies.h"

bool calc_frequencies(Frequencies freqs, const char* path, const char** a_error) {
	FILE* file = fopen(path, "r");
	if(file == NULL) {
		*a_error = strerror(errno);
		return false;
	}
	else {
		for(uchar ch = fgetc(file); !feof(file); ch = fgetc(file)) {
			(freqs[ch])++;
		}
	}
	fclose(file);
	return true;
}
