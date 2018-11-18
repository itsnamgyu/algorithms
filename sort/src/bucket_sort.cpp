#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

int BUCKET_EXP = 4;  // size = 2^exp;


static int get_max(int *array, int begin, int end);

void bucket_sort(int *array, int begin, int end) {
	unsigned int max = get_max(array, begin, end) - INT_MIN;
	unsigned int bucket_size = 1 << BUCKET_EXP;

	auto *buckets0 = new std::vector<unsigned int>[bucket_size];
	auto *buckets1 = new std::vector<unsigned int>[bucket_size];
	for (unsigned int i = 0; i < bucket_size; ++i) {
		buckets0[i] = std::vector<unsigned int>();
		buckets1[i] = std::vector<unsigned int>();
		buckets0[i].reserve((end - begin) / bucket_size);
		buckets1[i].reserve((end - begin) / bucket_size);
	}

	std::vector<unsigned int> *src = NULL;
	std::vector<unsigned int> *dst = NULL;

	unsigned int mask = (1 << BUCKET_EXP) - 1;

	for (unsigned int shift = 0; shift < 32 && (shift == 0 || max >> shift); shift += BUCKET_EXP) {
		if (shift == 0) {
			for (int i = begin; i <= end; ++i) {
				unsigned int value = array[i] - INT_MIN;
				buckets0[value & mask].push_back(value);
			}
			src = buckets0;
			dst = buckets1;
		} else {
			unsigned int shifted_mask = mask << shift;
			for (unsigned int i = 0; i < bucket_size; ++i) {
				for (auto const& value : src[i])
					dst[(value & shifted_mask) >> shift].push_back(value);
				src[i].clear();
			}

			auto *tmp = src;
			src = dst;
			dst = tmp;
		}
	}

	int index = begin;
	for (unsigned int i = 0; i < bucket_size; ++i) {
		for (auto const& value : src[i])
			array[index++] = value + INT_MIN;
	}
	
	delete[] buckets0;
	delete[] buckets1;
}

static int get_max(int *array, int begin, int end) {
	int max = INT_MIN;
	for (int i = begin; i <= end; ++i)
		if (array[i] > max) max = array[i];
	return max;
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
