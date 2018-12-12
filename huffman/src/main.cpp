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
	char CSV[] = "analysis.csv";
	FILE *csv = fopen(CSV, "w");
	if (csv == NULL) {
		printf("couldn't open analysis output file [%s]\n", CSV);
		return;
	}

	auto input_bits = BitSequence(in);

	fprintf(csv, "Symbol Size,Compressed,Table 1 (Length List),");
	fprintf(csv, "Table 2 (Symbol List),Total,Compression\n");

	int input_size = input_bits.size();
	printf("Original Data: %d bits\n", input_size);

	printf("%-10s%-10s%-10s%-10s%-10s%-10s\n",
			"Bits", "Data", "Table 1", "Table 2",
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
		int size1 = table1.size();
		printf("%-10d", size1);
		fprintf(csv, "%d,", size1);

		int total_size;

		if (i <= 16) {
			total_size = encoded.size() + (size0 < size1 ? size0 : size1);
		} else {
			total_size = encoded.size() + size1;
		}


		printf("%-10d", total_size);
		fprintf(csv, "%d,", total_size);

		printf("%-10.5f", (float) total_size / input_size);
		fprintf(csv, "%f,", (float) total_size / input_size);

		printf("\n");
		fprintf(csv, "\n");
	}
}
