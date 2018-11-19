#include <cstdio>
#include <cstdlib>

static int swaps = 0;
static const char FILENAME[] = "_hardsort.cpp";
static FILE *f;

void swap(int i, int j);

void merge(int i, int x, int j, int y);

void sort(int i, int m);


int main(int arg_count, char **args) {
	int N;

	if (arg_count != 2) {
		fprintf(stderr, "usage: ./a.out HARDSORT_MAX\n");
		return -1;
	} else {
		N = atoi(args[1]);
	}

	f = fopen(FILENAME, "w");
	if (f == NULL) throw "cannot open file";

	fprintf(f, "#include <cstdlib>\n");
	fprintf(f, "int HARDSORT_MAX = %d;\n", N);

	fprintf(f, "static inline void SWAP(int *array, const int a, const int b) {\n");
	fprintf(f, "\tint temp = array[a];\n");
	fprintf(f, "\tif (temp <= array[b]) return;\n");
	fprintf(f, "\tarray[a] = array[b];\n");
	fprintf(f, "\tarray[b] = temp;\n");
	fprintf(f, "}\n");

	for (int n = 1; n <= N; ++n) {
		swaps = 0;
		fprintf(f, "static void hardsort%d(int *array, int n) {\n", n);
    	sort(1, n);
		fprintf(f, "\n");
		fprintf(f, "}\n");
	}

	fprintf(f, "static void (*_hardsort[%d])(int*, int) = {\n", N + 1);
	fprintf(f, "\tNULL,");
	for (int n = 1; n <= N; ++n) {
		if (n  % 4 == 1)
			fprintf(f, "\n");
		fprintf(f, "\thardsort%d,", n);
	}
	fprintf(f, "\n");
	fprintf(f, "};\n");

	return 0;
}

void swap(int i, int j) {
	if (swaps == 4) {
		fprintf(f, "\n");
		swaps = 0;
	}
    fprintf(f, "\tSWAP(array, %d, %d); ", i - 1, j - 1);
	swaps += 1;
}

void merge(int i, int x, int j, int y) {
    int a, b;

	if (x == 1 && y == 1) {
		swap(i, j);
	} else if(x == 1 && y == 2) {
		swap(i, (j + 1));
		swap(i, j);
	} else if (x == 2 && y == 1) {
        swap(i, j);
        swap((i + 1), j);
    } else {
        a = x / 2;
         b = (x & 1) ? (y/2) : ((y + 1)/2);
        //b = y / 2;
        merge(i, a, j, b);
        merge((i + a), (x - a), (j + b), (y - b));
        merge((i + a), (x - a), j, b);
    }
}

void sort(int i, int m) {
    if (m > 1) {
        int a = m / 2;
        sort(i, a);
        sort((i + a), (m - a));
        merge(i, a, (i + a), (m - a));
    }
}
