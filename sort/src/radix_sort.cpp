#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

int BUCKET_EXP = 8;  // size = 2^exp;


static int get_max(int *array, int begin, int end);

void radix_sort(int *array, int begin, int end) {
#ifndef TEST
	if ((long long) end - begin < 2048) {
		insertion_sort(array, begin, end);
		return;
	}
#endif

	unsigned int max = get_max(array, begin, end) - INT_MIN;
	unsigned int bucket_size = 1 << BUCKET_EXP;

	auto *buckets0 = new std::vector<unsigned int>[bucket_size];
	auto *buckets1 = new std::vector<unsigned int>[bucket_size];
	for (unsigned int i = 0; i < bucket_size; ++i) {
		buckets0[i] = std::vector<unsigned int>();
		buckets1[i] = std::vector<unsigned int>();
	}

	std::vector<unsigned int> *src = NULL;
	std::vector<unsigned int> *dst = NULL;

	unsigned int mask = (1 << BUCKET_EXP) - 1;

	src = buckets0;
	for (unsigned int shift = 0; true; shift += BUCKET_EXP) {
		unsigned int next_shift = shift + BUCKET_EXP;
		bool first = (shift == 0);
		bool last = next_shift > 32 || (max >> next_shift) == 0;

		if (first) {
			for (int i = begin; i <= end; ++i) {
				unsigned int value = array[i] - INT_MIN;
				buckets0[value & mask].push_back(value);
			}
			src = buckets0;
			dst = buckets1;
		} else if (!last) {
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

		if (last) {
			int index = begin;
			for (unsigned int i = 0; i < bucket_size; ++i)
				for (auto const& value : src[i])
					array[index++] = value + INT_MIN;
			break;
		}
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
	assert(validate_sort(radix_sort, true));
	return 0;
}
#endif
