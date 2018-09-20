#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "input.h"
#include "spec.h"

#ifdef TEST
#undef TEST
int main() {
	printf("[manual] output 10 input numbers\n");
	{
		int *input = generate_input(10);
		printf("\t");
		dump_input(stdout, input, 10);
		free(input);
	}
	printf("[manual] generate \"test_input.txt\" with 10 inputs\n");
	generate_input_file("test_input.txt", 10);
	printf("[auto] write + save + read...\n");
	{
		for (int size_factor = 1; size_factor <= 24; ++size_factor) {
			int *input = generate_input((1 << size_factor));
			write_input("auto_test_input.txt", input, (1 << size_factor));

			int alt_size;
			int *alt_input = read_input("auto_test_input.txt", &alt_size);

			assert(alt_input);
			assert((1 << size_factor) == alt_size);

			
			int *cursor = input;
			int *alt_cursor = alt_input;
			for (int i = 0; i < (1 << size_factor); ++i)
				assert(*cursor++ == *alt_cursor++);

			free(input);
			free(alt_input);

			printf("\tsize of 2^%d successful\n", size_factor);
		}
	}
	printf("\tsuccess!\n");

	printf("[complete] \"%s\"\n", __FILE__);
	
	return 0;
}
#endif

int *generate_input(int size) {
	int *input = malloc(sizeof(int) * size);
	int *cursor = input;
	int range_size = INPUT_MAX - INPUT_MIN + 1;

	for (int i = 0; i < size; ++i)
		*cursor++ = INPUT_MIN + rand() % range_size;
	
	return input;
}

void write_input(char *filename, int *input, int size) {
	FILE *f = fopen(filename, "w");
	
	if (f == NULL) {
		fprintf(stderr, "couldn't open file\n");
		fprintf(stderr, DEBUG_EXPR);
		exit(-1);
	}

	fprintf(f, "%d ", size);
	dump_input(f, input, size);

	fclose(f);
}

void generate_input_file(char *filename, int size) {
	int *input = generate_input(size);
	write_input(filename, input, size);
	free(input);
}

int *read_input(char *filename, int *size) {
	FILE *f = fopen(filename, "r");
	
	if (f == NULL) {
		fprintf(stderr, "couldn't open file\n");
		fprintf(stderr, DEBUG_EXPR);
		return NULL;
	}

	if (fscanf(f, "%d ", size) != 1)
		return NULL;

	int *input = malloc(sizeof(int) * *size);
	int *cursor = input;
	for (int i = 0; i < *size; ++i) {
		if (fscanf(f, "%d ", cursor++) != 1) {
			free(input);
			return NULL;
		}
	}

	fclose(f);

	return input;
}

void dump_input(FILE *f, int *input, int size) {
	int *cursor = input;
	for (int i = 0; i < size; ++i)
		fprintf(f, "%d ", *cursor++);
	fprintf(f, "\n");
}
