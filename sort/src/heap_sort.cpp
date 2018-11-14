#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>

static int n;

static void adjust(int *array, int root);

static inline void swap(int *array, int a, int b);

void heap_sort(int *array, int begin, int end) {
	array = array + begin - 1;
	n = end - begin + 1;

	for (int i = n / 2; i >= 1; --i)
		adjust(array, i);

	while (n > 1) {
		swap(array, 1, n--);
		adjust(array, 1);
	}
}

static void adjust(int *array, int root) {
	// max heap
	int child = root * 2;
	int origin = array[root];

	while (child <= n) {
		if (child != n && array[child] < array[child + 1]) child++;
		if (origin < array[child]) array[child / 2] = array[child];
		else break;
		child *= 2;
	}

	array[child / 2] = origin;
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
	assert(validate_sort(heap_sort, true));

	return 0;
}
#endif
