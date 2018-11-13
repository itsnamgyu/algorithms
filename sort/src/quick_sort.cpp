#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>

/*
 * uses end as pivot element
 */
static int pivot_and_get_index(int *array, int begin, int end);

static inline void swap(int *array, int a, int b);

static inline int get_median(int *array, int a, int b, int c);

void quick_sort(int *array, int begin, int end) {
	if (begin >= end) return;
	int pivot = pivot_and_get_index(array, begin, end);
	quick_sort(array, begin, pivot - 1);
	quick_sort(array, pivot + 1, end);
}

void median_qs(int *array, int begin, int end) {
	if (begin >= end) return;

	int median_index = get_median(array, begin, (begin + end) / 2, end);
	swap(array, median_index, end);

	int pivot = pivot_and_get_index(array, begin, end);
	quick_sort(array, begin, pivot - 1);
	quick_sort(array, pivot + 1, end);
}

static int pivot_and_get_index(int *array, int begin, int end) {
	int to_push = begin;
	int to_pull = begin;
	int pivot = array[end];
	
	while (to_pull < end) {
		if (array[to_pull] < pivot)  // pull!
			swap(array, to_push++, to_pull++);
		else  // pull the next one?
			to_pull++;
	}

	swap(array, to_push, end);
	return to_push;
}

static inline void swap(int *array, int a, int b) {
	int temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}

static inline int get_median(int *array, int a, int b, int c) {
	int aa = array[a];
	int bb = array[b];
	int cc = array[c];

	int x = aa - bb;
    int y = bb - cc;
    int z = aa - cc;
    if (x * y > 0) return b;
    if (x * z > 0) return c;
    return a;
}

#ifdef TEST
#undef TEST
#include "sort.cpp"
#include "data.cpp"
int main() {
	assert(validate_sort(quick_sort, true));
	assert(validate_sort(median_qs, true));

	return 0;
}
#endif
