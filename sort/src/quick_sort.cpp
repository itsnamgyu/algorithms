#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>

/*
 * uses end as pivot element
 */
static int pivot_and_get_index(int *array, int begin, int end);

static inline void swap(int *array, int a, int b);

void quick_sort(int *array, int begin, int end) {
	if (begin >= end) return;
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

#ifdef TEST
#undef TEST
#include "sort.cpp"
#include "data.cpp"
int main() {
	assert(validate_sort(quick_sort, true));

	return 0;
}
#endif
