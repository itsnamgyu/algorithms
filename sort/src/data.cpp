#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <string>
#include <algorithm>

#include "../include/data.hpp"

Data::Data(const std::vector<int> &numbers) {
	this->numbers = numbers;
}

Data Data::random(int count) {
	srand(time(NULL));

	long range = (long) MAX - (long) MIN;
	auto d = Data(std::vector<int>(count));

	int random = rand();
	for (int i = 0; i < count; ++i) {
		random += rand();
		d.numbers[i] = MIN + (long) random % (long) range;
	}

	return d;
}

Data Data::load(char *filename) {
	FILE *f = fopen(filename, "r");

	if (f == NULL) 
		throw std::runtime_error(std::string("Can't open input file ") + std::string(filename));

	int n;
	if (fscanf(f, "%d", &n) != 1)
		throw std::runtime_error(std::string("Invalid input file ") + std::string(filename));

	auto numbers = std::vector<int>(n);
	for (int i = 0; i < n; ++i)
		if (fscanf(f, "%d", &numbers[i]) != 1)
			throw std::runtime_error(std::string("Invalid input file ") + std::string(filename));

	fclose(f);

	return Data(numbers);
}

bool Data::operator ==(const Data &other) const {
	return this->numbers == other.numbers;
}

bool Data::operator !=(const Data &other) const {
	return this->numbers != other.numbers;
}

void Data::save(char *filename) {
	FILE *f = fopen(filename, "w");

	if (f == NULL)
		throw std::runtime_error(std::string("Can't open output file ") + std::string(filename));

	fprintf(f, "%ld ", numbers.size());
	print(f);

	fclose(f);
}

void Data::print(FILE *f) {
	for (auto n : numbers)
		fprintf(f, "%d ", n);
}
void Data::sort(void (*sort)(int *array, int begin, int end)) {
	sort(numbers.data(), 0, numbers.size() - 1);
}

#ifdef TEST
#undef TEST

static void test_std_sort(int *array, int begin, int end) {
	std::sort(array + begin, array + end + 1);
}

static void test_random_print() {
	printf("[%s] manual random print test...\n", __FILE__);

	auto d = Data::random(10);
	d.print(stdout);
	printf("\n");
}

static void test_io() {
	printf("[%s] io test...\n", __FILE__);

	int _numbers[] = { 0, 24, 9358, -29483, 20384 };
	auto numbers = std::vector<int>(_numbers, std::end(_numbers));

	char filename[] = "test_data.txt";
	auto a = Data(numbers);
	a.save(filename);
	auto b = Data::load(filename);

	assert(a == b);
	assert(numbers == a.numbers);
	assert(numbers == b.numbers);
}

static void test_comparison() {
	printf("[%s] comparison test...\n", __FILE__);

	auto a = std::vector<int>();
	auto b = std::vector<int>();

	a.push_back(1);
	a.push_back(2);
	b.push_back(1);

	auto da = Data(a);
	auto db = Data(b);

	assert(a != b);
	assert(da != db);

	b.push_back(2);

	da = Data(a);
	db = Data(b);

	assert(a == b);
	assert(da == db);
}

void test_sort() {
	printf("[%s] data.sort test...\n", __FILE__);
	int _numbers[] = { 3, 2, 1 };
	auto numbers = std::vector<int>(_numbers, std::end(_numbers));
	auto data = Data(numbers);
	data.sort(test_std_sort);
	
	int _sorted_numbers[] = { 1, 2, 3 };
	auto sorted_numbers = std::vector<int>(_sorted_numbers, std::end(_sorted_numbers));
	auto sorted_data = Data(sorted_numbers);

	assert(sorted_data == data);
}

int main() {
	test_random_print();
	test_io();
	test_comparison();
	test_sort();
	return 0;
}
#endif
