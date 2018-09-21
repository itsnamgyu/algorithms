#include <stdio.h>
#include "input.h"

int main() {
	char outfile[100];
	
	for (int size_factor = 1; size_factor <= 20; ++size_factor) {
		int *input = generate_input((1 << size_factor));
		sprintf(outfile, "input%02d.txt", size_factor);

		printf("generating %s... ", outfile);
		write_input(outfile, input, (1 << size_factor));
		printf("done\n");
	}

	return 0;
}
