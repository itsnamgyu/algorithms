#pragma once

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>

typedef unsigned int uint;
typedef unsigned char uchar;


class Data {
	public:
		/*
		 * Basic constructors and destructors
		 */
		Data();
		Data(const uchar *data, int n);
		Data(const std::vector<uchar> data);
		~Data();

		/*
		 * Read n bytes from the specified stream. If n is set to
		 * -1, read until the end fo the file. MUST BE BINARY STREAM.
		 */
		static Data from_stream(FILE *stream, int n=-1);

		/*
		 * Basic comparison functions for testing etc.
		 */
		bool operator==(const Data& other) const;

		// private:  // maybe in a perfect world
		std::vector<uchar> data;

		/*
		 * For our compressed files, we will be dealing with individual bits.
		 * Hence, the last few bits of the last byte written to the file may
		 * not be used. We need to keep track of this.
		 */
		int unused_tail_bits;
};


class BitSequence {
	public:
		/*
		 * Basic constructors and destructors
		 */
		BitSequence();
		~BitSequence();

		/*
		 * Pack the individual bits in the BitSequence into a string of
		 * unsigned chars (wrapped in a Data object).
		 */
		Data compile() const;

		int get_length();

		void append(const BitSequence& tail);

		uchar* get_data();  // public access for easy modification

		bool operator==(const BitSequence& other) const;

	private:
		/*
		 * Vector of bits. Declared as uchar vector to avoid optimization
		 * and allow for array-like access.
		 */
		std::vector<uchar> data;
};
