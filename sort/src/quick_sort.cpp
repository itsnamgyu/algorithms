#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cstring>
#include <cassert>
#include <stack>
#include <climits>

static float MEAN_SHIFT_FACTOR = 0.25f;
static float INV_MEAN_SHIFT_FACTOR = 1.0f - MEAN_SHIFT_FACTOR;
/*
 * uses end as pivot element
 */
static int pivot_and_get_index(int *array, int begin, int end);

static int pivot_and_get_index_by_value(int *array, int begin, int end, int pivot);

static inline void swap(int *array, int a, int b);

static inline int get_median(int *array, int a, int b, int c);

static void local_sort(int *array, int begin, int end);

static std::pair<int,int> get_min_max(int *array, int begin, int end);

void quick_sort(int *array, int begin, int end) {
	if (begin >= end) return;
	int pivot = pivot_and_get_index(array, begin, end);
	quick_sort(array, begin, pivot - 1);
	quick_sort(array, pivot + 1, end);
}

static void _shifted_mean_qs(int *array, int begin, int end, int min, int max) {
	int median = array[get_median(array, begin, (begin + end) / 2, end)];
	int mean = (min + max) / 2 * INV_MEAN_SHIFT_FACTOR + median * MEAN_SHIFT_FACTOR;

	int mid_right = pivot_and_get_index_by_value(array, begin, end, mean);

	_shifted_mean_qs(array, begin, mid_right - 1, min, (mean + max) / 2);
	_shifted_mean_qs(array, mid_right, end, (min + mean) / 2, max);
}
void shifted_mean_qs(int *array, int begin, int end) {
	auto min_max = get_min_max(array, begin, end);
	_mean_qs(array, begin, end, min_max.first, min_max.second);
}

void median_qs(int *array, int begin, int end) {
	if (begin >= end) return;

	int median_index = get_median(array, begin, (begin + end) / 2, end);
	swap(array, median_index, end);

	int pivot = pivot_and_get_index(array, begin, end);
	median_qs(array, begin, pivot - 1);
	median_qs(array, pivot + 1, end);
}

void random_qs(int *array, int begin, int end) {
	if (begin >= end) return;
	int n = end - begin + 1;

	int random_index = begin + rand() % n;
	swap(array, random_index, end);

	int pivot = pivot_and_get_index(array, begin, end);
	random_qs(array, begin, pivot - 1);
	random_qs(array, pivot + 1, end);
}

void local_random_qs(int *array, int begin, int end) {
	local_sort(array, begin, end);
	random_qs(array, begin, end);
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

static int pivot_and_get_index_by_value(int *array, int begin, int end, int pivot) {
	int to_push = begin;
	int to_pull = begin;
	int pivot;
	
	while (to_pull <= end) {
		if (array[to_pull] < pivot)  // pull!
			swap(array, to_push++, to_pull++);
		else  // pull the next one?
			to_pull++;
	}

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

static void local_sort(int *array, int begin, int end) {
	int n = end - begin + 1;
	int *copy = new int[n + 1];
	memcpy(copy, array, n * sizeof(int));
	copy[n] = INT_MAX;
	array = array + begin;

	int i = 0;
	int j = 0;
	while (true) {
		int last = INT_MAX;
		while (copy[j] < last) last = copy[j++];

		int k = j - 1;
		while (i < j) {
			array[i++] = copy[k--];
		}

		if (j >= n) break;
	}

	delete[] copy;
}

static std::pair<int,int> get_min_max(int *array, int begin, int end) {
	int min = INT_MAX;
	int max = INT_MIN;
	int *p;

	for (int i = begin; i <= end; ++i, ++p) {
		if (*p < min) min = *p;
		else if (max < *p) max = *p;
	}
	
	return std::make_pair(min, max);
}

#ifdef TEST
#undef TEST
#include "sort.cpp"
#include "data.cpp"
int main() {
	assert(validate_sort(quick_sort, true));
	assert(validate_sort(median_qs, true));
	assert(validate_sort(random_qs, true));
	assert(validate_sort(local_random_qs, true));

	return 0;
}
#endif

