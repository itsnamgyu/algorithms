#include "../include/data.hpp"
#include <cassert>
#include <cctype>


ByteString::ByteString() = default;

ByteString::~ByteString() = default;

ByteString::ByteString(const uchar *data, int n) :
	data(data, data + n)
{}

ByteString::ByteString(const std::vector<uchar> &data) :
	data(data)
{}

ByteString ByteString::from_stream(FILE *stream, int n) {
	if (n == -1) {
		fseek(stream, 0, SEEK_END);
		n = ftell(stream);
		printf("file size: %d\n", n);
		fseek(stream, 0, SEEK_SET);
	}
	auto data = std::vector<uchar>(n);
	fread(&data[0], 1, n, stream);
    return ByteString(data);
}

bool ByteString::operator==(const ByteString &other) const {
	return data == other.data;
}

BitSequence::BitSequence() = default;

BitSequence::~BitSequence() = default;

BitSequence::BitSequence(uint data, size_t n) :
	BitSequence() {
	assert(0 <= n && n <= 32);
	for (int i = n - 1; i >= 0; --i)
		this->data.push_back((uchar) ((data >> i) & 1));
}

BitSequence::BitSequence(ByteString string) :
	BitSequence() {
	// add all bits from all bytes
	for (const auto &byte : string.data)
		for (int i = 7; i >= 0; --i)
			data.push_back((byte >> i) & 1);

	// remove unused tail bits from last byte
	for (int i = 0; i < string.tail_bits; ++i)
		data.pop_back();
}

BitSequence::BitSequence(FILE *stream, size_t n) :
	BitSequence(ByteString::from_stream(stream, n))
{}

BitSequence::BitSequence(std::vector<uint> primitives, int size) :
	BitSequence() {
	for (const auto &p : primitives)
		append(BitSequence(p, size));
}

BitSequence BitSequence::subset(int start, int end) const {
	BitSequence bits;
	if (start == -1) start = 0;
	if (end == -1) end = data.size();

	assert(0 <= start);
	assert(start <= end);
	assert(end <= data.size());

	bits.data = std::vector<uchar>(
			data.begin() + start,
			data.begin() + end);
	return bits;
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

uint BitSequence::to_primitive() const {
	size_t size = data.size();  // ignore method size()
	assert(size <= 32);
	
	uint primitive = 0;
	for (size_t i = 0; i < size; ++i) {
		primitive = primitive << 1;
		primitive += data[i] ? 1 : 0;
	}

	return primitive;
}

std::vector<uint> BitSequence::to_primitives(int size) const {
	size_t total_size = this->size();  // ignore method size()
	assert(total_size % size == 0);

	std::vector<uint> data;

	for (int head = 0; head < total_size; head += size) {
		uint primitive = 0;
		for (size_t i = 0; i < size; ++i) {
			primitive = primitive << 1;
			// note this->data
			primitive += this->data[head + i] ? 1 : 0;
		}
		data.push_back(primitive);
	}

	return data;
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

void BitSequence::print(FILE *stream) const {
	const int MAX = 32;
	fprintf(stream, "----------BitSequence----------\n");
	for (int head = 0; head < size() && head < MAX * 8; head += 8) {
		for (int i = 0; i < 8; ++i) {
			if (head + i < size())
				fprintf(stream, "%d", data[head + i]);
			else
				fprintf(stream, "-");
			if (i == 3) {
				fprintf(stream, " ");
			}
		}

		fprintf(stream, " | ");

		if (head + 4 <= size()) {
			uint h = subset(head, head + 4).to_primitive();
			fprintf(stream, "%X", h);
		}

		if (head + 8 <= size()) {
			uint h = subset(head + 4, head + 8).to_primitive();
			uint c = subset(head, head + 8).to_primitive();
			fprintf(stream, "%X", h);
			fprintf(stream, " | ");
			fprintf(stream, "%03d", c);
			fprintf(stream, " | ");
			if (isprint(c)) fprintf(stream, "%c", c);
			else fprintf(stream, "?");
		}

		fprintf(stream, "\n");
	}
	fprintf(stream, "----------%d of %d bytes----------\n", MAX, size() / 8);
}

void BitSequence::print_binary(FILE *stream) const {
	const int MAX = 32;
	for (int i = 0; i < data.size() && i < MAX; ++i) {
		fprintf(stream, "%d", data[i]);
		if (i % 4 == 3)
			fprintf(stream, " ");
	}
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
