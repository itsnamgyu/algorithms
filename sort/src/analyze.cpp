#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <algorithm>


#ifdef THOROUGH
#define THOROUGH 1
#else
#define THOROUGH 0
#endif

static sort_func sorters[] = {
	insertion_sort,
	std_sort,
	merge_sort,
	quick_sort,
	//ho_sort,
};

static char sorter_names[][100] = {
	"insertion_sort",
	"std_sort",
	"merge_sort",
	"quick_sort",
	//"ho_sort",
};

static sort_func main_sorters[] = {
	std_sort,
	merge_sort,
	//ho_sort,
};

static char main_sorter_names[][100] = {
	"std_sort",
	"merge_sort",
	//"ho_sort",
};

static int get_iter_count(int input_size);

static void test_linear(FILE *f_random, FILE *f_reverse);

static void test_log(FILE *f_random, FILE *f_reverse);

static void test_benchmark(FILE *f_random, FILE *f_reverse);

int main(void) {
	FILE *f_random = fopen("random_analysis.csv", "w");
	FILE *f_reverse = fopen("reverse_analysis.csv", "w");
	if (f_random == NULL) {
		printf("couldn't open random_analysis.csv\n");
		return -1;
	}
	if (f_reverse== NULL) {
		printf("couldn't open reverse_analysis.csv\n");
		return -1;
	}

	if (THOROUGH) Data::random(1 << 20).sort(std_sort);  // warm up CPU!

	const int n_sorters = sizeof(sorters) / sizeof(*sorters);
	const int n_main_sorters = sizeof(main_sorters) / sizeof(*main_sorters);

	fprintf(f_random, "input_size,");
	fprintf(f_reverse, "input_size,");
	for (int i = 0; i < n_sorters; ++i) {
		fprintf(f_random, "%s,", sorter_names[i]);
		fprintf(f_reverse, "%s,", sorter_names[i]);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	test_linear(f_random, f_reverse);
	test_log(f_random, f_reverse);


	for (int i = 0; i < n_main_sorters; ++i) {
		fprintf(f_random, "%s,", main_sorter_names[i]);
		fprintf(f_reverse, "%s,", main_sorter_names[i]);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	test_benchmark(f_random, f_reverse);

	fclose(f_random);
	fclose(f_reverse);
}

static int get_iter_count(int input_size) {
	int m = THOROUGH ? 5 : 1;

	if (input_size < 64)
		return 2000 * m;
	else if (input_size < 256)
		return 500 * m;
	else if (input_size < (1 << 10))
		return 50 * m;
	else if (input_size < (1 << 15))
		return 5 * m;
	else
		return 1 * m;
}

static void test_linear(FILE *f_random, FILE *f_reverse) {
	const int n_sorters = sizeof(sorters) / sizeof(*sorters);
	int max = THOROUGH ? 1024 : 256;

	printf("calculating times for input_size = 2 ~ %d\n", max);
	for (int input_size = 1; input_size < max; ++input_size) {
		if (input_size % 25 == 0) {
			printf("%.02f%% complete\n", (input_size - 2) / 253.0f * 100);
		}

		fprintf(f_random, "%d,", input_size);
		fprintf(f_reverse, "%d,", input_size);

		auto time_random = std::vector<double>(n_sorters, 0);
		auto time_reverse = std::vector<double>(n_sorters, 0);

		int iters = get_iter_count(input_size);
		for (int i = 0; i < iters; ++i) {
			Data data = Data::random(input_size);
			for (int index = 0; index < n_sorters; ++index) {
				if (input_size > (1 << 15) && index == 0) {
					printf("continue\n");
					continue;
				}
				Data d = data;
				time_random[index] += d.sort(sorters[index]);
				std::reverse(data.numbers.begin(), data.numbers.end());
				time_reverse[index] += d.sort(sorters[index]);
			}
		}
		
		for (int index = 0; index < n_sorters; ++index) {
			fprintf(f_random, "%lf,", time_random[index] / iters * 1000);
			fprintf(f_reverse, "%lf,", time_reverse[index] / iters * 1000);
		}
		fprintf(f_random, "\n");
		fprintf(f_reverse, "\n");
	}
	printf("done\n");
}

static void test_log(FILE *f_random, FILE *f_reverse) {
	const int n_sorters = sizeof(sorters) / sizeof(*sorters);
	int max_power = THOROUGH ? 28 : 24;
	int max_square_power = THOROUGH ? 18 : 16;

	printf("calculating times for input_size = 2^1 ~ 2^%d\n", max_power);
	for (int input_size = 1; input_size <= (1 << max_power); input_size = input_size << 1) {
		printf("calculating times for input_size = %d\n", input_size);

		fprintf(f_random, "%d,", input_size);
		fprintf(f_reverse, "%d,", input_size);
		
		auto time_random = std::vector<double>(n_sorters, 0);
		auto time_reverse = std::vector<double>(n_sorters, 0);

		int iters = get_iter_count(input_size);

		for (int i = 0; i < iters; ++i) {
			Data data = Data::random(input_size);
			for (int index = 0; index < n_sorters; ++index) {
				if (input_size > (1 << max_square_power) && index == 0)
					continue;  // skip N^2 algorithsm = insertion sort
				Data d = data;
				time_random[index] += d.sort(sorters[index]);
				std::reverse(data.numbers.begin(), data.numbers.end());
				if (input_size > (1 << max_square_power) && index == 3)
					continue;  // skip original quick sort on reverse
				time_reverse[index] += d.sort(sorters[index]);
			}
		}
		
		for (int index = 0; index < n_sorters; ++index) {
			fprintf(f_random, "%lf,", time_random[index] / iters * 1000);
			fprintf(f_reverse, "%lf,", time_reverse[index] / iters * 1000);
		}
		fprintf(f_random, "\n");
		fprintf(f_reverse, "\n");
		
		fflush(f_random);
		fflush(f_reverse);
	}
	printf("done\n");
}

static void test_benchmark(FILE *f_random, FILE *f_reverse) {
	const int n_main_sorters = sizeof(main_sorters) / sizeof(*main_sorters);
	int input_size = 1 << 20;
	int iters = THOROUGH ? 500 : 50;

	auto time_random = std::vector<double>(n_main_sorters, 0);
	auto time_reverse = std::vector<double>(n_main_sorters, 0);

	printf("calculating times for input_size = 2^20\n");
	for (int i = 0; i < iters; ++i) {
		if ((iters + 1) % 10 == 0)
			printf("%02d of %02d\n", i, iters);

		Data data = Data::random(input_size);
		for (int index = 0; index < n_main_sorters; ++index) {
			Data d = data;
			time_random[index] += d.sort(main_sorters[index]);
			std::reverse(data.numbers.begin(), data.numbers.end());
			time_reverse[index] += d.sort(main_sorters[index]);
		}
	}
	
	for (int index = 0; index < n_main_sorters; ++index) {
		fprintf(f_random, "%lf,", time_random[index] / iters * 1000);
		fprintf(f_reverse, "%lf,", time_reverse[index] / iters * 1000);
		printf("%s: %lf, %lf\n", main_sorter_names[index],
				time_random[index] / iters * 1000,
				time_reverse[index] / iters * 1000);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	
	fflush(f_random);
	fflush(f_reverse);
	printf("done\n");
}
