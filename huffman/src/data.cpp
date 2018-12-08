#include "../include/data.hpp"
#include <cassert>


ByteString::ByteString() = default;

ByteString::~ByteString() = default;

ByteString::ByteString(const uchar *data, int n) :
	data(data, data + n)
{}

ByteString::ByteString(const std::vector<uchar> data) :
	data(data)
{}

ByteString ByteString::from_stream(FILE *stream, int n) {
	std::vector<uchar> data(n);
	fread(&data[0], 1, n, stream);

    return ByteString(data);
}

bool ByteString::operator==(const ByteString &other) const {
	return data == other.data;
}

BitSequence::BitSequence() = default;

BitSequence::~BitSequence() = default;

BitSequence::BitSequence(uint data, size_t n) :
	data() {
	assert(0 <= n && n <= 32);
	for (int i = n - 1; i >= 0; --i)
		this->data.push_back((uchar) ((data >> i) & 1));
}

ByteString BitSequence::compile() const {
	size_t size = get_length();
	size_t bytes;
	size_t tail_bits;
	
	if (size % 8 == 0) {
		bytes = size / 8;
		tail_bits = 0;
	} else {
		bytes = size / 8 + 1;
		tail_bits = size % 8;
	}

	auto string= ByteString(std::vector<uchar>(0, bytes));
	string.tail_bits = tail_bits;

	uchar *current = &string.data[0];

	for (size_t i = 0; i < size; ++i) {
		*current <<= 1;
		*current += data[i] ? 1 : 0;
		if (i % 8 == 7) *current += 1;
	}

	*current <<= tail_bits;

	return string;
}

int BitSequence::get_length() const {
    return data.size();
}

void BitSequence::append(const BitSequence &tail) {
	data.insert(data.end(),
				tail.data.begin(),
				tail.data.end());
}

uchar *BitSequence::get_data() {
    return &data[0];
}

bool BitSequence::operator==(const BitSequence &other) const {
    return data == other.data;
}
