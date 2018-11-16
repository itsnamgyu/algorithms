#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

int BUCKET_SIZE = 10;

void _bucket_sort(int *array, int begin, int end) {
}
void bucket_sort(int *array, int begin, int end) {
	if (end - begin < (1 << 10)) {
		insertion_sort(array, begin, end);
		return;
	} else if (end - begin < (1 << 15)) {
		heap_sort(array, begin, end);
		return;
	}

	auto *_buckets = new std::vector<int>((long long) INT_MAX - INT_MIN + 1, 0);
	int *buckets = _buckets->data() - INT_MIN;
	
	for (int i = begin; i <= end; ++i)
		++buckets[array[i]];

	int index = 0;
	for (long long i = INT_MIN; i <= INT_MAX; ++i)
		while (buckets[i]--)
			array[index++] = i;

	delete _buckets;
}

#ifdef TEST
#undef TEST
#include "sort.cpp"
#include "data.cpp"
#include "insertion_sort.cpp"
#include "heap_sort.cpp"
int main() {
	assert(validate_sort(bucket_sort, true));
	return 0;
}
#endif
