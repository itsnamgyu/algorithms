#pragma once
#include <cstdio>
#include <vector>

class Data {
	public:
		Data(const std::vector<int> &number);

		static Data load(char *filename);
		static Data random(int count);

		std::vector<int> numbers;

		void save(char *filename);
		void print(FILE *f);
		bool operator ==(const Data &b) const;
};
