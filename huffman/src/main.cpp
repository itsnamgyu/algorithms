#include <cstdio>
#include <cstdlib>

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

	auto bits = BitSequence(in);
	bits.print();
	auto rebits = BitSequence(bits.compile());
	assert(bits == rebits);
	BitSequence(in).print();

	auto *tree = PrefixTree::from_content(bits, 16);
	auto book = CodeBook(tree);
	auto *canon_tree = book.to_canonical_prefix_tree();
	auto canon_book= CodeBook(canon_tree);
	assert(book == canon_book);

	{
		auto bits = book.to_code_length_list();
		auto new_book = book.from_code_length_list(
				bits, book.get_length_size());

		printf("Code Length List Size:             %d\n", bits.size());

		assert(book == new_book);
	}

	{
		auto bits = book.to_counted_length_symbol_list();
		auto new_book = book.from_counted_length_symbol_list(
				bits, book.get_symbol_size(), book.get_count_size(),
				book.get_max_code_length());

		printf("Counted Length Symbol List Size:   %d\n", bits.size());

		assert(book == new_book);
	}

	auto encoded = book.encode(bits);
	auto decoded = canon_tree->decode(encoded);
	printf("----------Table----------\n");
	//book.print_code_table();
	
	printf("Original Size:  %d\n", bits.size());
	printf("Encoded:        %d\n", encoded.size());

	printf("Original\n");
	//bits.print();
	printf("Encoded\n");
	//encoded.print();
	printf("Decoded\n");
	//decoded.print();



	assert(bits == decoded);


	// open output file if needed
	if (mode == C || mode == D) {
		int size = strlen(filename) + strlen(out_ext) + 1;
		char *out_filename = new char[size];
		out_filename[0] = '\0';
		strcat(out_filename, filename);
		strcat(out_filename, out_ext);
		printf("generating output file [%s]...\n", out_filename);

		if ((out = fopen(out_filename, "wb")) == NULL) {
			printf("couldn't open output file %s\n", out_filename);
			exit(-1);
		}
		delete[] out_filename;
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
}

void decompress(FILE *in, FILE *out) {
}

void analyze(FILE *in) {
}
