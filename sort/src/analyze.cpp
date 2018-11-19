#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>


static bool DEEP = false;

enum TimeComplexity {
	LIN,
	LOG,    // NlogN worst case
	ALOG,   // NlogN average case (N^2 on reverse)
	SQR,    // N^2 on average case
};
static sort_func sorters[] = {
	std_sort,
	merge_sort,
	heap_sort,
	quick_sort,
	insertion_sort,
	bucket_sort,

	median_qs,
	random_qs,
	local_random_qs,
	shifted_mean_qs,

	early_quick_sort,
	early_median_qs,
	early_random_qs,
	early_local_random_qs,
	early_shifted_mean_qs,
};
static char sorter_names[][100] {
	"std_sort",
	"merge_sort",
	"heap_sort",
	"quick_sort",
	"insertion_sort",
	"bucket_sort",

	"median_qs",
	"random_qs",
	"local_random_qs",
	"shifted_mean_qs",

	"early_quick_sort",
	"early_median_qs",
	"early_random_qs",
	"early_local_random_qs",
	"early_shifted_mean_qs",
};
static TimeComplexity sorter_times[] = {
	LOG, LOG, LOG, ALOG, SQR, LIN,
	LOG, LOG, LOG, LOG,
	ALOG, LOG, LOG, LOG, LOG,
};
static const int n_sorters = sizeof(sorters) / sizeof(*sorters);


static sort_func main_sorters[] = {
	std_sort,
	merge_sort,
	heap_sort,
	quick_sort,
	median_qs,
	insertion_sort,
	bucket_sort,
};
static char main_sorter_names[][100] {
	"std_sort",
	"merge_sort",
	"heap_sort",
	"quick_sort",
	"median_qs",
	"insertion_sort",
	"bucket_sort",
};
static const int n_main_sorters = sizeof(main_sorters) / sizeof(*main_sorters);


static int get_iter_count(int input_size);

static void analyze_low();

static void analyze_general();

static void analyze_linear(FILE *f_random, FILE *f_reverse);

static void analyze_log(FILE *f_random, FILE *f_reverse);

static void analyze_benchmark(FILE *f_random, FILE *f_reverse);

static void print_center(const char *string, int n = 80);


int main(int arg_count, char **args) {
	switch (arg_count) {
		case 1:
			print_center(std::string("Running General Analysis").c_str());
			analyze_general();
			return 0;
		case 2:
			if (!strcmp(args[1], "deep")) {
				DEEP = true;;
				print_center(std::string("Running Deep General Analysis").c_str());
				analyze_general();
				return 0;
			}
			if (!strcmp(args[1], "low")) {
				print_center(std::string("Running Low Index Analysis").c_str());
				analyze_low();
				return 0;
			}
	}

	printf("usage: ./a.out options\n");
	printf("options:\n");
	printf("default: general analysis\n");
	printf("deep: thorough general analysis\n");
	printf("low: low index analysis\n");
}

static void analyze_low() {
	int max = 128;
	
	FILE *f_random = fopen("random_low.csv", "w");
	FILE *f_reverse = fopen("reverse_low.csv", "w");
	if (f_random == NULL) throw "couldn't open random_low.csv";
	if (f_reverse== NULL) throw "couldn't open reverse_low.csv";

	fprintf(f_random, "input_size,");
	fprintf(f_reverse, "input_size,");
	for (int i = 0; i < n_main_sorters; ++i) {
		fprintf(f_random, "%s,", main_sorter_names[i]);
		fprintf(f_reverse, "%s,", main_sorter_names[i]);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");

	printf("calculating times for input_size = 2 ~ %d\n", max);
	printf("\n");
	for (int input_size = 1; input_size <= max; ++input_size) {
		if ((input_size + 1) % 10 == 0)
			printf("\r%.2f%% complete\n", (float) (input_size - 2) / (max - 2) * 100);

		fprintf(f_random, "%d,", input_size);
		fprintf(f_reverse, "%d,", input_size);

		auto time_random = std::vector<double>(n_main_sorters, 0);
		auto time_reverse = std::vector<double>(n_main_sorters, 0);

		int iters = get_iter_count(input_size);
		for (int i = 0; i < iters; ++i) {
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
		}
		fprintf(f_random, "\n");
		fprintf(f_reverse, "\n");
	}

	fclose(f_random);
	fclose(f_reverse);
}

static void analyze_general() {
	FILE *f_random = fopen("random_general.csv", "w");
	FILE *f_reverse = fopen("reverse_general.csv", "w");
	if (f_random == NULL) throw "couldn't open random_general.csv";
	if (f_reverse== NULL) throw "couldn't open reverse_general.csv";

	if (DEEP) Data::random(1 << 20).sort(std_sort);  // warm up CPU!

	fprintf(f_random, "input_size,");
	fprintf(f_reverse, "input_size,");
	for (int i = 0; i < n_sorters; ++i) {
		fprintf(f_random, "%s,", sorter_names[i]);
		fprintf(f_reverse, "%s,", sorter_names[i]);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	analyze_linear(f_random, f_reverse);
	analyze_log(f_random, f_reverse);


	for (int i = 0; i < n_sorters; ++i) {
		if (sorter_times[i] == SQR) continue;
		fprintf(f_random, "%s,", sorter_names[i]);
		if (sorter_times[i] == ALOG) continue;
		fprintf(f_reverse, "%s,", sorter_names[i]);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	analyze_benchmark(f_random, f_reverse);

	fclose(f_random);
	fclose(f_reverse);
}

static int get_iter_count(int input_size) {
	int m = DEEP ? 10 : 1;

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

static void analyze_linear(FILE *f_random, FILE *f_reverse) {
	int max = DEEP ? 256 : 128;

	printf("calculating times for input_size = 2 ~ %d\n", max);
	printf("\n");
	for (int input_size = 1; input_size <= max; ++input_size) {
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

static void analyze_log(FILE *f_random, FILE *f_reverse) {
	const int n_sorters = sizeof(sorters) / sizeof(*sorters);
	int max_power = DEEP ? 28 : 20;
	int max_square_power = DEEP ? 18 : 15;

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

static void analyze_benchmark(FILE *f_random, FILE *f_reverse) {
	int input_size = 1 << 20;
	int iters = DEEP ? 250 : 20;

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

static void print_center(const char *string, int n) {
	int remaining = n - strlen(string);
	for (int i = 0; i < remaining / 2; ++i)
		printf("-");
	printf("%s", string);
	for (int i = 0; i < (remaining + 1) / 2; ++i)
		printf("-");
	printf("\n");
}
