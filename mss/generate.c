#include <stdio.h>
#include <stdlib.h>
#include "generate.h"

int *generate_input(int size) {
	int *input = malloc(sizeof(int) * size);
	int *cursor = input;
	int range_size = INPUT_MAX - INPUT_MIN + 1;

	for (int i = 0; i < size; ++i, ++cursor)
		*cursor = INPUT_MIN + rand() % range_size;
	
	return input;
}

void generate_input_file(char *filename, int size) {
	FILE *f = fopen(filename, "w");
	
	if (f == NULL) {
		fprintf(stderr, "couldn't open file\n");
		fprintf(stderr, DEBUG_EXPR);
		exit(-1);
	}

	fprintf(f, "%d ", size);

	int *inputs = generate_inputs(size);
	for (int i = 0; i < size; ++i)
		fprintf(f, "%d ", inputs[i]);
	free(inputs);
}
