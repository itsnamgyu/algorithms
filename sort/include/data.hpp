#pragma once
#include <vector>

class Data {
	public:
		Data(const std::vector<int> &number);

		static Data load(char *filename);

		std::vector<int> numbers;

		void save(char *filename);
		bool Data::operator ==(const Data &b) const;
};
