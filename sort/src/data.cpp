#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <string>

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

#ifdef TEST
#undef TEST

void test_random_print() {
	printf("[%s] manual random print test...\n", __FILE__);

	auto d = Data::random(10);
	d.print(stdout);
	printf("\n");
}

void test_io() {
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

void test_comparison() {
	printf("[%s] comparison test...\n", __FILE__);

	auto a = std::vector<int>();
	auto b = std::vector<int>();

	a.push_back(1);
	a.push_back(2);
	b.push_back(1);

	assert(a != b);

	b.push_back(2);

	assert(a == b);
}

int main() {
	test_random_print();
	test_io();
	test_comparison();
	return 0;
}
#endif
