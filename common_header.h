#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H
//Include Standard Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

// Use inttypes.h instead of stdint.h
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
//#include <stdint.h>
#include <stdbool.h>
//Math Libraries

//#include <math.h>
#include <limits.h>
//#include <gsl/gsl_sf_bessel.h>
//RNG Libraries:
#include <linux/random.h>

//POSIX libraries:
#include <unistd.h>
#include <pthread.h>

//Ncurses
//#include <ncurses.h>
#endif
