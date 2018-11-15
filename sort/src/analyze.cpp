#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <algorithm>


#ifdef ALL
#define ALL 1
#else
#define ALL 0
#endif

enum TimeComplexity {
	LOG,  // NlogN worst case
	ALOG,  // NlogN average case (N^2 on reverse)
	SQR,  // N^2 on average case
};

static sort_func sorters[] = {
	std_sort,
	merge_sort,
	heap_sort,
	quick_sort,
	median_qs,
	insertion_sort,
};
static char sorter_names[][100] {
	"std_sort",
	"merge_sort",
	"heap_sort",
	"quick_sort",
	"median_qs",
	"insertion_sort",
};
static TimeComplexity sorter_times[] = {
	LOG,
	LOG,
	LOG,
	ALOG,
	ALOG,
	SQR
};
static const int n_sorters = sizeof(sorters) / sizeof(*sorters);


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

	if (ALL) Data::random(1 << 20).sort(std_sort);  // warm up CPU!

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


	for (int i = 0; i < n_sorters; ++i) {
		if (sorter_times[i] == SQR) continue;
		fprintf(f_random, "%s,", sorter_names[i]);
		if (sorter_times[i] == ALOG) continue;
		fprintf(f_reverse, "%s,", sorter_names[i]);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	test_benchmark(f_random, f_reverse);

	fclose(f_random);
	fclose(f_reverse);
}

static int get_iter_count(int input_size) {
	int m = ALL ? 10 : 1;

	if (input_size < 32)
		return 10000 * m;
	if (input_size < 64)
		return 2500 * m;
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
	int max = ALL ? 1024 : 256;

	printf("calculating times for input_size = 2 ~ %d\n", max);
	printf("\n");
	for (int input_size = 1; input_size < max; ++input_size) {
		if ((input_size + 1) % 32 == 0)
			printf("\r%.2f%% complete\n", (float) (input_size - 2) / (max - 2) * 100);

		fprintf(f_random, "%d,", input_size);
		fprintf(f_reverse, "%d,", input_size);

		auto time_random = std::vector<double>(n_sorters, 0);
		auto time_reverse = std::vector<double>(n_sorters, 0);

		int iters = get_iter_count(input_size);
		for (int i = 0; i < iters; ++i) {
			Data data = Data::random(input_size);
			for (int index = 0; index < n_sorters; ++index) {
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
}

static void test_log(FILE *f_random, FILE *f_reverse) {
	const int n_sorters = sizeof(sorters) / sizeof(*sorters);
	int max_power = ALL ? 28 : 24;
	int max_square_power = ALL ? 18 : 16;

	printf("calculating times for input_size = 2^1 ~ 2^%d\n", max_power);
	printf("\n");
	for (int power = 0; power <= max_power; ++power) {
		int input_size = 1 << power;
		printf("\rcalculating times for input_size = 2^%02d = %d\n", power, input_size);

		fprintf(f_random, "%d,", input_size);
		fprintf(f_reverse, "%d,", input_size);
		
		auto time_random = std::vector<double>(n_sorters, 0);
		auto time_reverse = std::vector<double>(n_sorters, 0);

		int iters = get_iter_count(input_size);

		for (int i = 0; i < iters; ++i) {
			Data data = Data::random(input_size);
			for (int index = 0; index < n_sorters; ++index) {
				if (input_size > (1 << max_square_power) && sorter_times[index] == SQR)
					continue;  // skip N^2 algorithsm
				Data d = data;
				time_random[index] += d.sort(sorters[index]);
				std::reverse(data.numbers.begin(), data.numbers.end());
				if (input_size > (1 << max_square_power) && sorter_times[index] == ALOG)
					continue;  // skip worst N^2 algorithms
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
}

static void test_benchmark(FILE *f_random, FILE *f_reverse) {
	int input_size = 1 << 20;
	int iters = ALL ? 500 : 50;

	auto time_random = std::vector<double>(n_sorters, 0);
	auto time_reverse = std::vector<double>(n_sorters, 0);

	printf("calculating times for input_size = 2^20\n");
	printf("\n");
	for (int i = 0; i < iters; ++i) {
		printf("\r%02d of %02d\n", i, iters);

		Data data = Data::random(input_size);
		for (int index = 0; index < n_sorters; ++index) {
			if (sorter_times[index] == SQR) continue;
			Data d = data;
			time_random[index] += d.sort(sorters[index]);
			std::reverse(data.numbers.begin(), data.numbers.end());

			if (sorter_times[index] == ALOG) continue;
			time_reverse[index] += d.sort(sorters[index]);
		}
	}
	
	for (int index = 0; index < n_sorters; ++index) {
		if (sorter_times[index] == SQR) continue;

		printf("\n%-15s", sorter_names[index]);

		fprintf(f_random, "%lf,", time_random[index] / iters * 1000);
		printf("%20.6lf", time_random[index] / iters * 1000);

		if (sorter_times[index] == ALOG) continue;

		fprintf(f_reverse, "%lf,", time_reverse[index] / iters * 1000);
		printf("%20.6lf", time_reverse[index] / iters * 1000);
	}

	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	printf("\n");
	
	fflush(f_random);
	fflush(f_reverse);
}
