#include "../include/hardsort.hpp"
#include "../include/sorter.hpp"

#include "_hardsort.cpp"

void hardsort(int *array, int n) {
	if (n <= 1) return;
	_hardsort[n](array, n);
}

void pure_hard_sort(int *array, int begin, int end) {
	if (end <= begin) return;

	const int n = end - begin + 1;
	if (n <= HARDSORT_MAX)
		hardsort(array + begin, n);
	else
		throw "hardsort size exceeded";
}

#ifdef TEST
#undef TEST
#include "sort.cpp"
#include "data.cpp"
int main() {
	assert(validate_sort(pure_hard_sort, true, HARDSORT_MAX));

	return 0;
}
#endif
