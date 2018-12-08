#include "../include/data.hpp"


Data::Data() = default;

Data::~Data() = default;

Data::Data(const uchar *data, int n) :
	data(data, data + n)
{}

Data::Data(const std::vector<uchar> data) :
	data(data)
{}

Data Data::from_stream(FILE *stream, int n) {
	std::vector<uchar> data(n);
	fread(&data[0], 1, n, stream);

    return Data(data);
}

bool Data::operator==(const Data &other) const {
	return data.data == other.data;
}

BitSequence::BitSequence() = default;

BitSequence::~BitSequence() = default;

Data BitSequence::compile() const {
    return Data();
}

int BitSequence::get_length() {
    return data.size();
}

void BitSequence::append(const BitSequence &tail) {
	data.insert(data.end(), tail.data.begin(), tail.data.end());
}

uchar *BitSequence::get_data() {
    return &data[0];
}

bool BitSequence::operator==(const BitSequence &other) const {
    return data == other.data;
}
