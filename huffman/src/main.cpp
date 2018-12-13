#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <cstring>

#include "../include/data.hpp"
#include "../include/code.hpp"


void print_usage();

void compress(FILE *in, FILE *out);

void decompress(FILE *in, FILE *out);

void analyze(FILE *in);

int main(int arg_count, char **args) {
	if (arg_count != 3) {
		print_usage();
		return 0;
	}

	// extensions for output files
	char *out_ext;
	char zz[] = ".zz";
	char yy[] = ".yy";

	enum Mode {
		C, D, A
	} mode;
	if (!strcmp(args[1], "-c")) {
		mode = C;
		out_ext = zz;
	} else if (!strcmp(args[1], "-d")) {
		mode = D;
		out_ext = yy;
	} else if (!strcmp(args[1], "-a")) {
		mode = A;
	} else {
		print_usage();
		return 0;
	}

	FILE *in = nullptr;
	FILE *out = nullptr;
	char *filename = args[2];

	// open input file
	if ((in = fopen(filename, "rb")) == NULL) {
		printf("couldn't open input file %s\n", filename);
		exit(-1);
	}

	// open output file if needed
	if (mode == C || mode == D) {
		int size = strlen(filename) + strlen(out_ext) + 1;
		char *out_filename = new char[size];
		out_filename[0] = '\0';
		strcat(out_filename, filename);
		strcat(out_filename, out_ext);
		printf("[%s] -> [%s]\n", filename, out_filename);

		if ((out = fopen(out_filename, "wb")) == NULL) {
			printf("couldn't open output file %s\n", out_filename);
			exit(-1);
		}
		delete[] out_filename;
		printf("\n");
	}

	switch (mode) {
		case C: compress(in, out); break;
		case D: decompress(in, out); break;
		case A: analyze(in); break;
	}

	return 0;
}

void print_usage() {
	printf("usage:\n");
	printf("compression:     mp3_20161662 -c <datafile>\n");
	printf("decompression:   mp3_20161662 -d <compressed_file.zz>\n");
	printf("analysis:        mp3_20161662 -a <datafile>\n");
}

void compress(FILE *in, FILE *out) {
	// all methods require 20 bits of metadata to
	// store compression parameters
	const int METADATA_SIZE = 20;

	auto input_bits = BitSequence(in);

	// determine optimal compression parameters
	int min_size = input_bits.size() + 8;  // extra byte for flag
	int symbol_size = -1;
	int book_format = -1;  // {0|1}
	if (input_bits.size() != 0) {
		printf("Determining Optimal Parameters\n");
		printf("---------------------------------------------------------------\n");
		for (int i = 4; i <= 24; ++i) {
			// build Huffman Tree
			auto *tree = PrefixTree::from_content(input_bits, i);
			auto book = CodeBook(tree);
			free(tree);

			// encode data
			auto encoded = book.encode(input_bits);
			printf("#"); fflush(stdout);

			int size;

			// generate CodeBook format 1 if applicable
			if (i <= 16) {
				auto table0  = book.to_code_length_list();
				size = encoded.size() + table0.size() + METADATA_SIZE;
				if (size < min_size) {
					min_size = size;
					symbol_size = i;
					book_format = 0;
				}
			}
			printf("#"); fflush(stdout);

			// generate CodeBook format 2
			auto table1 = book.to_counted_length_symbol_list();
			// note that the number of lengths need to be saved
			// in the compressed data, which requires up to
			// symbol_size bits. (in addition to METADATA)
			size = encoded.size() + table1.size() + i + METADATA_SIZE;
			if (size < min_size) {
				min_size = size;
				symbol_size = i;
				book_format = 1;
			}
			printf("#"); fflush(stdout);
		}
		printf("\n");
		printf("---------------------------------------------------------------\n");
		printf("\n");
	}


	if (symbol_size == -1) {
		printf("Disabling compression\n");
	} else {
		printf("Compression Parameters\n");
		printf("---------------------------------------------------------------\n");
		printf("Symbol Size:   %d\n", symbol_size);
		printf("Book Format:   %s\n",
				book_format == 0 ?  "Length List" : "Symbol List");
		printf("---------------------------------------------------------------\n");
	}
	printf("\n");


	if (symbol_size == -1) {
		printf("Saving data with no-compression flag... ");
		auto output_bits = BitSequence();
		output_bits.append(BitSequence((uint) 0, (int) 8));
		output_bits.append(input_bits);
		output_bits.write(out);
		printf("complete.\n");

	} else {
		printf("Compressing... "); fflush(stdout);
		auto *tree = PrefixTree::from_content(input_bits, symbol_size);
		auto book = CodeBook(tree);
		auto encoded = book.encode(input_bits);
		free(tree);
		printf("complete!\n");
		
		
		printf("Generating code book... "); fflush(stdout);
		BitSequence table;
		if (book_format == 0)
			table  = book.to_code_length_list();
		else
			table = book.to_counted_length_symbol_list();
		printf("complete!\n");


		printf("Saving... "); fflush(stdout);
		auto output_bits = BitSequence();
		int alignment_bits = (symbol_size - (input_bits.size() % symbol_size))
			% symbol_size;
		int tail_bits = (8 - (min_size % 8) % 8);

		// metadata
		output_bits.append(BitSequence(1, 1));  // toggle compression
		output_bits.append(BitSequence(symbol_size, 5));
		output_bits.append(BitSequence(book_format, 1));
		output_bits.append(BitSequence(alignment_bits, 5));
		output_bits.append(BitSequence(tail_bits, 3));
		if (book_format == 0)
			output_bits.append(BitSequence(book.get_length_size(), 5));
		else
			output_bits.append(BitSequence(book.get_count_size(), 5));

		// confirm metadata size
		assert(output_bits.size() == METADATA_SIZE);

		// add additional metdata for symbol list
		if (book_format == 1)
			output_bits.append(BitSequence(book.get_max_code_length(), symbol_size));

		output_bits.append(table);	// code book
		output_bits.append(encoded);  // encoded data
		assert(output_bits.size() == min_size);  // confirm total size
		output_bits.write(out);  // save data

		printf("complete!\n");
		printf("---------------------------------------------------------------\n");
		printf("Compression Rate: %.2f%%\n",
				(float) output_bits.size() / input_bits.size() * 100);
	}
}

void decompress(FILE *in, FILE *out) {
	auto bits = BitSequence(in);

	printf("Decompressing... ");  fflush(stdout);
	if (bits[0]) {
		int symbol_size = bits.subset(1, 6).to_primitive();
		int table_method = bits.subset(6, 7).to_primitive();
		int alignment_bits = bits.subset(7, 12).to_primitive();
		int tail_bits = bits.subset(12, 15).to_primitive();
		int book_misc = bits.subset(15, 20).to_primitive();
		int last_bit = bits.size() - tail_bits;
		
		BitSequence decoded;
		if (table_method == 0) {
			//             length_size * n_possible_symbols
			int table_size = book_misc * (1 << symbol_size);
			auto table_bits = bits.subset(20, 20 + table_size);
			auto book = CodeBook::from_code_length_list(
					table_bits, book_misc);
			auto *tree = book.to_canonical_prefix_tree();
			auto encoded = bits.subset(20 + table_size, last_bit);
			decoded = tree->decode(encoded);
			free(tree);
		} else {
			int table_start = 20 + symbol_size;
			int max_length = bits.subset(20, table_start).to_primitive();

			// get symbol_count
			//              count_size* max_length
			int left_size = book_misc * max_length;
			auto left = bits.subset(table_start, table_start + left_size);
			auto symbol_counts = left.to_primitives(book_misc);
			int symbol_count = 0;
			for (const auto &n : symbol_counts)
				symbol_count += (int) n;

			int right_size = symbol_count * symbol_size;
			int table_size = left_size + right_size;
			auto table_bits = bits.subset(table_start, table_start + table_size);
			auto book = CodeBook::from_counted_length_symbol_list(
					table_bits, symbol_size, book_misc, max_length);
			auto *tree = book.to_canonical_prefix_tree();
			auto encoded = bits.subset(table_start + table_size, last_bit);
			decoded = tree->decode(encoded);
			free(tree);
		}
		while (alignment_bits--)
			decoded.data.pop_back();
		decoded.write(out);
	} else {
		auto decoded = bits.subset(8, -1);
		decoded.write(out);
	}
	printf("complete!\n");
}

void analyze(FILE *in) {
	char CSV[] = "analysis.csv";
	FILE *csv = fopen(CSV, "w");
	if (csv == NULL) {
		printf("couldn't open analysis output file [%s]\n", CSV);
		return;
	}

	auto input_bits = BitSequence(in);

	fprintf(csv, "Symbol Size,Data,Book 1 (Length List),");
	fprintf(csv, "Book 2 (Symbol List),Total,Compression\n");

	int input_size = input_bits.size();
	printf("Original Data: %d bits\n", input_size);

	printf("%-10s%-10s%-10s%-10s%-10s%-10s\n",
			"Bits", "Data", "Book 1", "Book 2",
			"Total", "Compression");

	for (int i = 4; i <= 24; ++i) {
		auto *tree = PrefixTree::from_content(input_bits, i);
		auto book = CodeBook(tree);

		free(tree);

		printf("%-10d", i);
		fprintf(csv, "%d,", i);

		auto encoded = book.encode(input_bits);
		printf("%-10d", encoded.size());
		fprintf(csv, "%d,", encoded.size());

		int size0;
		if (i <= 16) {
			auto table0 = book.to_code_length_list();
			size0 = table0.size();
			printf("%-10d", size0);
			fprintf(csv, "%d,", size0);
		} else {
			printf("%-10s", "N/A");
			fprintf(csv, "%s,", "N/A");
		}

		auto table1 = book.to_counted_length_symbol_list();
		int size1 = table1.size() + book.get_symbol_size();
		printf("%-10d", size1);
		fprintf(csv, "%d,", size1);

		// consider metadata size
		int total_size;
		if (i <= 16) {
			total_size = encoded.size() + (size0 < size1 ? size0 : size1) + 20;
		} else {
			total_size = encoded.size() + size1 + 20;;
		}


		printf("%-10d", total_size);
		fprintf(csv, "%d,", total_size);

		printf("%-10.5f", (float) total_size / input_size);
		fprintf(csv, "%f,", (float) total_size / input_size);

		printf("\n");
		fprintf(csv, "\n");
	}
}
