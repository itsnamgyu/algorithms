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

BitSequence::BitSequence(ByteString string) :
	data() {
	// add all bits from all bytes
	for (const auto &byte : string.data)
		for (int i = 7; i >= 0; --i)
			data.push_back((byte >> i) & 1);

	// remove unused tail bits from last byte
	for (int i = 0; i < string.tail_bits; ++i)
		data.pop_back();
}

ByteString BitSequence::compile() const {
	size_t size = data.size();  // ignore method size()
	size_t bytes;
	size_t tail_bits;
	
	if (size % 8 == 0) {
		bytes = size / 8;
		tail_bits = 0;
	} else {
		bytes = size / 8 + 1;
		tail_bits = 8 - size % 8;
	}

	assert(tail_bits < 8);

	auto string = ByteString(std::vector<uchar>(bytes, 0));
	string.tail_bits = tail_bits;

	uchar *current = &string.data[0];

	for (size_t i = 0; i < size; ++i) {
		*current = *current << 1;
		*current += data[i] ? 1 : 0;
		if (i % 8 == 7) current += 1;
	}

	*current = *current << tail_bits;

	return string;
}

int BitSequence::size() const {
    return data.size();
}

void BitSequence::append(const BitSequence &tail) {
	data.insert(data.end(),
				tail.data.begin(),
				tail.data.end());
}

void BitSequence::append(uchar bit) {
	data.push_back(bit);
}

uchar *BitSequence::get_data() {
    return &data[0];
}

bool BitSequence::operator==(const BitSequence &other) const {
    return data == other.data;
}

uchar &BitSequence::operator[](const int index) {
	return data[index];
}
