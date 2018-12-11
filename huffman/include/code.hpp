#pragma once

#include <utility>
#include "data.hpp"


class Symbol {
	public:
		Symbol(int code_length, BitSequence &symbol);

		Symbol(int code_length, uint symbol, int size);

		BitSequence to_bits() const;

		bool operator==(const Symbol &other) const;

		int code_length;

		/*
		 * Supports up to 32-bit symbols; for the case of Huffman
		 * Coding, it's only feasible to use up to 16-bits per bit
		 * string anyways.
		 */
		uint symbol;
		int size;
};

class PrefixTree {
	public:
		/*
		 * Construct a raw PrefixTree: for use in various algorithms
		 */
		PrefixTree();

		/*
		 * Construct a single symbol PrefixTree leaf
		 */
		PrefixTree(BitSequence symbol, int frequency);

		/*
		 * Construct a single symbol from Symbol
		 */
		PrefixTree(const Symbol &symbol);

		/*
		 * Generate a valid PrefixTree via the Huffman Code algorithm for
		 * some data. The original data should be in complete bytes; i.e.,
		 * 8*N bits long.
		 */
		static PrefixTree *from_content(BitSequence &bits, int symbol_size);

		/*
		 * Should recursively destroy children; there's no other
		 * choice... Declared virtual to make PrefixTree a
		 * polymorphic class for RTTI.
		 */
		virtual ~PrefixTree(); 

		/*
		 * Decode data using the PrefixTree.
		 */
		BitSequence decode(BitSequence &bits) const;

		BitSequence symbol;

		int frequency;

		PrefixTree *children[2];
};

class CodeBook {
	public:
		CodeBook();

		/*
		 * Create a code book based on a valid prefix tree.
		 */
		explicit CodeBook(PrefixTree *tree);

		/*
		 * Initialize code book based on binary representations.
		 */
		static CodeBook from_code_length_list(
				BitSequence &bits, int length_size);

		static CodeBook from_counted_length_symbol_list(
				BitSequence &bits, int symbol_size,
				int count_size, int max_code_length);

		/*
		 * Convert to valid prefix tree with canonical Huffman Code.
		 */
		PrefixTree *to_canonical_prefix_tree();

		/*
		 * Convert to binary representations.
		 */
		BitSequence to_code_length_list();
		BitSequence to_counted_length_symbol_list();

		/*
		 * Encode data using the CodeBook
		 */
		BitSequence encode(BitSequence &bits) const;

		/*
		 * code_table[symbol] = (BitSequence) code;
		 */
		std::vector<BitSequence> get_code_table() const;

		/*
		 * Print code table for debugging purposes
		 */
		void print_code_table() const;

		/*
		 * Comparison for testing purposes
		 */
		bool operator==(const CodeBook &other) const;

		/*
		 * Number of bits required to represent symbol lengths
		 * TODO: redundant recalculation (for all of the below)
		 */
		int get_length_size() const;

		/* 
		 * Number of bits required to represent number of symbols
		 * at each code length.
		 */
		int get_count_size() const;

		/*
		 * symbol_counts[i]: number of symbols of length i - 1.
		 * Pre-step for get_count_size
		 */
		int get_count_size(const std::vector<uint> &symbol_counts) const;

		int get_symbol_size() const;

		int get_max_code_length() const;
	// private:
		// symbol_counts[i]: number of symbols of length i - 1
		std::vector<uint> get_symbol_counts() const;

		std::vector<Symbol> symbols;
};
