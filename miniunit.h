#ifndef __MINIUNIT_H__
#define __MINIUNIT_H__

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "clog.h"

#define mu_check_strings_eq(s1, s2) mu_check(strcmp((s1), (s2)) == 0)

#define __mu_log_err_color(color, ...) \
do { \
	if(isatty(STDERR_FILENO)) { \
		fprintf(stderr, "%s", color); \
	} \
	fprintf(stderr, __VA_ARGS__); \
	if(isatty(STDERR_FILENO)) { \
		fprintf(stderr, "%s", ANSI_RESET); \
	} \
} while(false); 

#define mu_start() int __mu_line_num = 0

#define mu_check(condition)                               \
	do {                                                  \
		if((condition) == false && __mu_line_num == 0) {  \
			__mu_line_num = __LINE__;                     \
		}                                                 \
} while(false)

#define mu_run(function) do {                                                            \
	if(function() == 0) {                                                                \
	__mu_log_err_color(ANSI_GREEN, "Test passed: %s\n", #function);                      \
	} else {                                                                             \
    __mu_log_err_color(ANSI_RED, "Test failed: %s at line %d\n", #function, function()); \
	}                                                                                    \
} while(false)
	
#define mu_end() return __mu_line_num                                

#endif
