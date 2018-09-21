#include <stdlib.h>
#include <limits.h>
#include "mss.h"
#include "spec.h"


#ifdef TEST
#undef TEST
#include "input.h"
#include "input.c"
int main() {
	printf("[auto] compare results between different algos...\n");
	{
		int max = 2500;
		int dec = max / 10;
		for (int i = 1; i <= max; ++i) {
			mss_result *a;
			mss_result *b;
			int *input = generate_input(i);

			a = mss_n2(input, i);
			b = mss_n(input, i);

			assert(a->sum == b->sum);
			assert(a->start== b->start);
			assert(a->end== b->end);

			free(a);
			free(b);
			free(input);

			if (i % dec == 0)
				printf("\t%d of %d\n", i, max);
		}
		printf("\tall match!\n");
	}

	printf("[complete] %s\n", __FILE__);
	return 0;
}
#endif

mss_result *mss_n2(int *data, int size) {
	mss_result *result = malloc(sizeof(mss_result));
	result->sum = INT_MIN;
	
	for (int i = 0; i < size; ++i) {
		int sum = 0;
		for (int j = i; j < size; ++j) {
			sum += data[j];
			if (sum > result->sum) {
				result->sum = sum;
				result->start = i;
				result->end = j;
			}
		}
	}

	return result;
}

mss_result *mss_nlogn(int *data, int size);

mss_result *mss_n(int *data, int size) {
	mss_result *result = malloc(sizeof(mss_result));
	result->sum = INT_MIN;

	int sum = 0;
	int start = 0;
	result->start = start;
	for (int i = 0; i < size; ++i) {
		sum += data[i];
		if (sum > result->sum) {
			result->sum = sum;
			result->start = start;
			result->end = i;
		}
		if (sum < 0) {
			start = i + 1;
			sum = 0;
		}
	}

	return result;
}
