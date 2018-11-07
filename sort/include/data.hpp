#pragma once
#include <vector>

class Data {
	public:
		Data(const std::vector<int> &number);

		static Data *load_data(char *filename);

		std::vector<int> numbers;

		bool Data::operator ==(const Data &b) const;
};
