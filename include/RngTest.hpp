#ifndef RNGTEST_H
#define RNGTEST_H

#include "Rng.hpp"

#include <math.h>
#include <iostream>
#include <array>
#include <cassert>

#define SAMPLE_SIZE 1000
#define BOOL_GOAL 0.5
#define BOOL_DIFF 0.1
#define TRUE_PROB 0.2
#define TRUE_DIFF 0.05
#define NUMVAL_GOAL 0
#define NUMVAL_DIFF 0.1
#define ITERATIONS_GOAL 50
#define ITERATIONS_DIFF 5
#define FITNESS_LOW 0.1
#define FITNESS_HIGH 0.9
#define FITNESS_LOW_FACTOR 6
#define FITNESS_HIGH_FACTOR 12

void testRng();

#endif // RNGTEST_H