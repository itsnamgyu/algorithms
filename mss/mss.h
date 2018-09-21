#pragma once
#include "spec.h"

/*
 * Functions that solve mss at different time complexities.
 * Adheres to mss_function signature.
 */

mss_result mss_n2(int *data, int size);

mss_result mss_nlogn(int *data, int size);

mss_result mss_n(int *data, int size);
