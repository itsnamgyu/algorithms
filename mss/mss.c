#include <stdlib.h>
#include <limits.h>
#include "mss.h"
#include "spec.h"

static mss_result mss_recursive(int *data, int left, int right);

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
			mss_result a, b, c;
			int *input = generate_input(i);

			a = mss_n2(input, i);
			b = mss_nlogn(input ,i);
			c = mss_n(input, i);

			assert(a.sum == b.sum);
			assert(b.sum == c.sum);
			assert(a.start == b.start);
			assert(b.start == c.start);
			assert(a.end == b.end);
			assert(b.end == c.end);

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

mss_result mss_n2(int *data, int size) {
	mss_result result;
	result.sum = INT_MIN;
	
	for (int i = 0; i < size; ++i) {
		int sum = 0;
		for (int j = i; j < size; ++j) {
			sum += data[j];
			if (sum > result.sum) {
				result.sum = sum;
				result.start = i;
				result.end = j;
			}
		}
	}

	return result;
}

mss_result mss_nlogn(int *data, int size) {
	return mss_recursive(data, 0, size - 1);
}

mss_result mss_n(int *data, int size) {
	mss_result result;
	result.sum = INT_MIN;

	int sum = 0;
	int start = 0;
	result.start = start;
	for (int i = 0; i < size; ++i) {
		sum += data[i];
		if (sum > result.sum) {
			result.sum = sum;
			result.start = start;
			result.end = i;
		}
		if (sum < 0) {
			start = i + 1;
			sum = 0;
		}
	}

	return result;
}


static mss_result mss_recursive(int *data, int left, int right) {
	if (left == right) {
		mss_result result;
		result.sum = data[left];
		result.start = left;
		result.end = left;
		return result;
	}

	int center = (left + right) / 2;

	mss_result results[3];
	results[0] = mss_recursive(data, left, center);
	results[2] = mss_recursive(data, center + 1, right);

	results[1].start = center;
	results[1].end = center;
	results[1].sum = data[center];
	
	int sum = data[center];
	int index = center - 1;
	while (index >= left) {
		sum += data[index];
		if (results[1].sum <= sum) {
			results[1].sum = sum;
			results[1].start = index;
		}
		index--;
	}

	sum = results[1].sum;
	index = center + 1;
	while (index <= right) {
		sum += data[index];
		if (results[1].sum < sum) {
			results[1].sum = sum;
			results[1].end = index;
		}
		index++;
	}

	mss_result max_result = results[0];
	for (int i = 1; i < 3; ++i)
		if (max_result.sum < results[i].sum)
			max_result = results[i];

	return max_result;
}
