#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <algorithm>
#include <cassert>

void std_sort(Data &data) {
	std::sort(data.numbers.begin(), data.numbers.end());
}

void null_sort(Data &data) {
}

bool sorts_correctly(sort_func sort, int iterations) {
	for (int i = 0; i < iterations; ++i) {
		Data data = Data::random(i);
		Data copy = data;

		sort(data);
		std_sort(copy);

		if (data != copy) return false;
	}

	return true;
}

#ifdef TEST
#undef TEST
#include "data.cpp"

void test_null_sort() {
	printf("[%s] null_sort test...\n", __FILE__);

	for (int i = 0; i < 1000; ++i) {
		auto a = Data::random(i);
		auto b = a;

		null_sort(a);
		assert(a == b);
	}
}

void test_std_sort() {
	printf("[%s] std_sort test...\n", __FILE__);

	int _numbers[] = { 5, 4, 3, 2, 1, 0 };
	int _sorted_numbers[] = { 0, 1, 2, 3, 4, 5 };
	auto numbers = std::vector<int>(_numbers, std::end(_numbers));
	auto sorted_numbers = std::vector<int>(_sorted_numbers, std::end(_sorted_numbers));

	auto a = Data(numbers);
	auto b = Data(sorted_numbers);

	std_sort(a);

	assert(a == b);
}

void test_sorts_correctly() {
	printf("[%s] sorts_correctly test...\n", __FILE__);

	assert(!sorts_correctly(null_sort));
	assert(sorts_correctly(std_sort));
}

int main() {
	test_null_sort();
	test_std_sort();
	test_sorts_correctly();
		
	return 0;
}
#endif
