#pragma once
#include <cstdio>
#include <vector>
#include <cstdlib>


class Data {
	public:
		static const int MAX = INT_MAX;
		static const int MIN = INT_MIN;

		Data();
		Data(const std::vector<int> &number);

		static Data load(char *filename);
		static Data random(int count);

		std::vector<int> numbers;

		void save(char *filename);
		void print(FILE *f);
		double sort(void (*sort)(int *array, int begin, int end));
		bool operator ==(const Data &b) const;
		bool operator !=(const Data &b) const;
};
