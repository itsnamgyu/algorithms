#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "spec.h"
#include "input.h"
#include "mss.h"


int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "expected 2 arguments\n");
		return -1;
	}
	
	int size;
	int *data = read_input(argv[1], &size);
	if (!data) {
		fprintf(stderr, "invalid input file\n");
		return -1;
	}

	int index;
	if (sscanf(argv[2], "%d", &index) != 1) {
		fprintf(stderr, "second argument invalid\n");
		return -1;
	}
	
	mss_result result;
	clock_t start_t = clock();
	switch (index) {
		case 0:
			result = mss_n2(data, size);
			break;
		case 1:
			result = mss_nlogn(data, size);
			break;
		case 2:
			result = mss_n(data, size);
			break;
		default:
			fprintf(stderr, "%d is not a valid algorithm index\n", index);
			return -1;
	}
	float duration = (float)(clock() - start_t) / CLOCKS_PER_SEC * 1000;

	char outfile[100];
	sprintf(outfile, "%s%s", "result_", argv[1]);

	FILE *f = fopen(outfile, "w");
	if (f == NULL) {
		fprintf(stderr, "could not open outfile %s\n", outfile);
		return -1;
	}

	fprintf(f, "%s\n", argv[1]);
	fprintf(f, "%d\n", index);
	fprintf(f, "%d\n", size);
	fprintf(f, "%d\n", result.start);
	fprintf(f, "%d\n", result.end);
	fprintf(f, "%d\n", result.sum);
	fprintf(f, "%f\n", duration);
	fclose(f);
}
