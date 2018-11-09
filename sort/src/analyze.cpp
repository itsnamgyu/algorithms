#include "../include/sorter.hpp"
#include "../include/data.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

static sort_func sorters[] = {
	insertion_sort,
	merge_sort,
	quick_sort,
	std_sort,
	//ho_sort,
	NULL,
};

static char sorter_names[][20] = {
	"insertion_sort",
	"merge_sort",
	"quick_sort",
	"std_sort",
	"ho_sort",
};

int main(void) {
	static const int ITER_VERY_MANY = 1000;
	static const int ITER_MANY = 1;
	static const int ITER_FEW = 1;

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

	fprintf(f_random, "input_size,");
	fprintf(f_reverse, "input_size,");
	for (int i = 0; i < 4; ++i) {
		fprintf(f_random, "%s,", sorter_names[i]);
		fprintf(f_reverse, "%s,", sorter_names[i]);
	}
	fprintf(f_random, "\n");
	fprintf(f_reverse, "\n");
	
	printf("calculating times for input_size = 2 ... 2048\n");
	for (int input_size = 2; input_size < 4096; ++input_size) {
		if (input_size % 102 == 0) {
			printf("%.02f%% complete\n", (input_size - 2) / 4093.0f * 100);
		}

		fprintf(f_random, "%d,", input_size);
		fprintf(f_reverse, "%d,", input_size);
		double time_random = 0;
		double time_reverse = 0;
		for (int index = 0; index < 4; ++index) {
			for (int i = 0; i < ITER_MANY; ++i) {
				Data data = Data::random(input_size);
				time_random += data.sort(sorters[index]);
				std::reverse(data.numbers.begin(), data.numbers.end());
				time_reverse += data.sort(sorters[index]);
			}
			fprintf(f_random, "%lf,", time_random / ITER_MANY * 1000);
			fprintf(f_reverse, "%lf,", time_reverse / ITER_MANY * 1000);
		}
		fprintf(f_random, "\n");
		fprintf(f_reverse, "\n");
	}
	printf("done\n");

	for (int input_size = 1024; input_size < (1 << 21); input_size = input_size << 1) {
		printf("calculating times for input_size = %d\n", input_size);

		fprintf(f_random, "%d,", input_size);
		fprintf(f_reverse, "%d,", input_size);
		double time_random = 0;
		double time_reverse = 0;
		for (int index = 0; index < 4; ++index) {
			for (int i = 0; i < ITER_FEW; ++i) {
				Data data = Data::random(input_size);
				if (input_size > (1 << 18) && index == 0) continue;
				time_random += data.sort(sorters[index]);
				std::reverse(data.numbers.begin(), data.numbers.end());
				time_reverse += data.sort(sorters[index]);
			}
			fprintf(f_random, "%lf,", time_random / ITER_MANY * 1000);
			fprintf(f_reverse, "%lf,", time_reverse / ITER_MANY * 1000);
		}
		fprintf(f_random, "\n");
		fprintf(f_reverse, "\n");
	}

	fclose(f_random);
	fclose(f_reverse);
}
