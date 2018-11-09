#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>

void insertion_sort(int *array, int begin, int end) {
	for (int i = begin + 1; i <= end; ++i) {
		int j;
		int current = array[i];
		for (j = i; j > begin; --j) {
			if (array[j - 1] > current) array[j] = array[j - 1];
			else break;
		}
		array[j] = current;
	}
}

#ifdef TEST
#undef TEST
#include "sort.cpp"
#include "data.cpp"
int main() {
	assert(validate_sort(insertion_sort, true));

	return 0;
}
#endif
