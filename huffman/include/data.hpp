#pragma once

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>

typedef unsigned int uint;
typedef unsigned char uchar;


class ByteString {
	public:
		/*
		 * Basic constructors and destructors
		 */
		ByteString();
		ByteString(const uchar *data, int n);
		ByteString(const std::vector<uchar> &data);
		~ByteString();

		/*
		 * Read n bytes from the specified stream. If n is set to
		 * -1, read until the end fo the file. MUST BE BINARY STREAM.
		 *  TODO: change to constructor?
		 */
		static ByteString from_stream(FILE *stream, int n=-1);

		/*
		 * Basic comparison functions for testing etc.
		 */
		bool operator==(const ByteString& other) const;

		// private:  // maybe in a perfect world
		std::vector<uchar> data;

		/*
		 * For our compressed files, we will be dealing with individual bits.
		 * Hence, the last few bits of the last byte written to the file may
		 * not be used. We need to keep track of this.
		 */
		int tail_bits = 0;
};


class BitSequence {
	public:
		BitSequence();
		~BitSequence();

		/*
		 * Generate BitSequence from n LSB's (NOT MSB) from an unsigned
		 * int.
		 */
		BitSequence(uint data, size_t n);

		/*
		 * Generate BitSequence from ByteString.
		 * TODO: add unit test
		 */
		BitSequence(ByteString string);

		/*
		 * Read n BYTES from the specified stream.
		 */
		BitSequence(FILE *stream, size_t n=-1);
		
		/*
		 * Generate BitSequence from list of primitives
		 * - size: number of LSBs to use (8 == char, 16 == short, etc)
		 * TODO: add unit test
		 */
		BitSequence(std::vector<uint> primitives, int size);

		/*
		 * Write contents to file stream. Note that 0-valued tail bits will
		 * be appended to the data to align to bytes. You are required to
		 * encode/decode the number of tail bits in the internal data.
		 */
		void write(FILE *stream);

		/*
		 * Pack the individual bits in the BitSequence into a string of
		 * unsigned chars (wrapped in a ByteString object).
		 */
		ByteString compile() const;

		/*
		 * Get subset of BitSequence. -1 should be considered as endpoints.
		 * TODO: solve major optimization issue...
		 */
		BitSequence subset(int start=-1, int end=-1) const;

		/*
		 * Pack the individual bits into a primitive (that fits within
		 * uint). If the BitSequence is longer than 32, will throw
		 * an exception.
		 */
		uint to_primitive() const;

		std::vector<uint> to_primitives(int size) const;

		int size() const;

		void append(const BitSequence& tail);

		void append(uchar bit);

		void print(FILE *stream = stdout) const;

		void print_binary(FILE *stream = stdout) const;

		uchar* get_data();  // public access for easy modification

		bool operator==(const BitSequence& other) const;

		uchar& operator[](const int index);

	// private:  // maybe in a perfect world
		/*
		 * Vector of bits. Declared as uchar vector to avoid optimization
		 * and allow for array-like access.
		 */
		std::vector<uchar> data;
};
