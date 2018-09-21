#pragma once

/*
 * Answers for the given mss problem.
 *
 * Note that the start, end indices are both inclusive.
 */
typedef struct _mss_result {
	int start;
	int end;
	int sum;
} mss_result;

/*
 * A function that takes inputs for the mss problem, and returns
 * an mss_result containing the answers. Returns NULL on invalid input.
 */
typedef mss_result mss_function(int *data, int size);

#define DEBUG_EXPR "line %d of \"%s\" function <%s>\n",__LINE__,__FILE__,__func__
