#pragma once
#include <cstdio>
#include <vector>
#include <cstdlib>


class Data {
	public:
		static const int MAX = INT_MAX;
		static const int MIN = INT_MIN;

		Data(const std::vector<int> &number);

		static Data load(char *filename);
		static Data random(int count);

		std::vector<int> numbers;

		void save(char *filename);
		void print(FILE *f);
		bool operator ==(const Data &b) const;
};
