#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstdio>

static sort_func sorters[5] = {
	NULL,
	insertion_sort,
	merge_sort,
	quick_sort,
	//ho_sort,
	NULL,
};

static void print_usage();

static void print_invalid_algo_index();

int main(int argument_count, char **arguments) {
	if (argument_count != 3) {
		print_usage();
		return -1;
	}

	Data data;
	try {
		data = Data::load(arguments[1]);
	} catch (const std::exception& e) {
		printf("error: couldn't open file %s\n", arguments[1]);
		return -1;
	}

	int algo_index;
	if (sscanf(arguments[2], "%d", &algo_index) != 1) {
		print_usage();
		return -1;
	}

	if (algo_index < 1 || 4 < algo_index) {
		print_invalid_algo_index();
		return -1;
	}

	double time = data.sort(sorters[algo_index]);
	
	char out_filename[1000];
	sprintf(out_filename, "result_%d_%s", algo_index, arguments[1]);

	FILE *f = fopen(out_filename, "w");
	if (f == NULL) {
		printf("couldn't open outfile %s\n", out_filename);
		return -1;
	}

	fprintf(f, "%s\n", arguments[1]);
	fprintf(f, "%d\n", algo_index);
	fprintf(f, "%lu\n", data.numbers.size());
	fprintf(f, "%.6lf\n", time);
	data.print(f);

	return 0;
}

static void print_usage() {
	printf("Usage: mp2_20161662 input_filename.txt algo_index\n");
}

static void print_invalid_algo_index() {
	printf("error: algo index must be in the range [1, 4].\n");
}
