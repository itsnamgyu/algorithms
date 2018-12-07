#pragma once

#include <utility>
#include "data.hpp"


class PrefixTree {
	public:
		/*
		 * Generate a valid PrefixTree via the Huffman Code algorithm for
		 * some data. The original data should be in complete bytes (like
		 * ordinary files).
		 */
		static PrefixTree from_content(Data &data, int symbol_size);

		/*
		 * Should recursively destroy children; there's no other
		 * choice... Declared virtual to make PrefixTree a
		 * polymorphic class for RTTI.
		 */
		virtual ~PrefixTree(); 

		/*
		 * Decode data using the PrefixTree.
		 */
		BitSequence decode(BitSequence bits) const;

		int frequency;

		uint symbol;

		PrefixTree *children[2];

	private:
		/*
		 * Incidates whether this is a complete PrefixTree
		 * following the Huffman Code algorithm. Internal
		 * PrefixNodes are not valid.
		 */
		bool valid;

		/*
		 * The size of each bit-string. The number of LSB's we'll be
		 * using within public member `symbol`.
		 */
		int symbol_size;
};


class Symbol {
	public:
		Symbol(int code_length, uint symbol);

		const int code_length;

		/*
		 * Supports up to 32-bit symbols; for the case of Huffman
		 * Coding, it's only feasible to use up to 16-bits per bit
		 * string anyways.
		 */
		const uint symbol;
};


class CodeBook {
	public:
		/*
		 * Create a code book based on a valid prefix tree.
		 */
		CodeBook(PrefixTree *tree);

		/*
		 * Convert to valid prefix tree with canonical Huffman Code.
		 */
		static PrefixTree to_canonical_prefix_tree();

		/*
		 * Initialize code book based on binary representations.
		 */
		static CodeBook from_code_length_list(
				BitSequence &bits, int symbol_size);
		static CodeBook from_counted_length_symbol_list(
				BitSequence &bits, int symbol_size);

		/*
		 * Convert to binary representations.
		 */
		BitSequence to_code_length_list();
		BitSequence to_counted_length_symbol_list();

		/*
		 * Encode data according to canonical Huffman code.
		 */
		BitSequence encode(Data &data);

		bool operator==(const CodeBook& other) const;

	private:
		std::vector<Symbol> symbols;
};
