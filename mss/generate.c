#include <stdio.h>
#include <stdlib.h>
#include "generate.h"
#include "spec.h"

#ifdef TEST
#undef TEST
int main() {
	printf("manual test: output 10 input numbers\n");
	int *input = generate_input(10);
	int *cursor = input;
	for (int i = 0; i < 10; ++i)
		printf("%d ", *cursor++);
	printf("\n");

	printf("\nmanual test: generate \"test_input.txt\" with 10 inputs\n");
	generate_input_file("test_input.txt", 10);

	printf("\nend of test\n");
	
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

void generate_input_file(char *filename, int size) {
	FILE *f = fopen(filename, "w");
	
	if (f == NULL) {
		fprintf(stderr, "couldn't open file\n");
		fprintf(stderr, DEBUG_EXPR);
		exit(-1);
	}

	fprintf(f, "%d ", size);

	int *inputs = generate_input(size);
	int *cursor = inputs;
	for (int i = 0; i < size; ++i)
		fprintf(f, "%d ", *cursor++);
	free(inputs);
}
