#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>

static void merge(int *array, int begin, int mid, int end);

void merge_sort(int *array, int begin, int end) {
	if (begin >= end) return;
	int mid = (begin + end) / 2;

	merge_sort(array, begin, mid);
	merge_sort(array, mid + 1, end);

	merge(array, begin, mid, end);
}

static void merge(int *array, int begin, int mid, int end) {
	int count = end - begin + 1;
	int *copy = new int[count];

	// merge sublists
	int i = 0;
	int a = begin;
	int b = mid + 1;
	while (i < count) {
		if (a != mid + 1 && (b == end + 1 || array[a] < array[b]))
			copy[i++] = array[a++];
		else
			copy[i++] = array[b++];
	}

	// copy from temp list and delete old
	memcpy(array + begin, copy, count * sizeof(int));
	delete[] copy;
}

#ifdef TEST
#undef TEST
#include "sort.cpp"
#include "data.cpp"
int main() {
	assert(validate_sort(merge_sort, true));

	return 0;
}
#endif
