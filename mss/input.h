#define INPUT_MAX 1000
#define INPUT_MIN -1000

/*
 * Generates input for mss with the specified size.
 * Returns a **dynamically allocated** int array containing
 * the inputs. The numbers are within the range of
 * [INPUT_MIN, INPUT_MAX]
 *
 * Rationale
 * To be able to generate input during runtime (without an
 * intermediate file for IO) for test purposes.
 */
int *generate_input(int size);

/*
 * Write the generated input to a file in the format specified by
 * spec.
 */
void write_input(char *filename, int *input, int size);

/*
 * As the name suggests. (generate + save input)
 */
void generate_input_file(char *filename, int size);

/*
 * Reads input from the input file and returns the size of
 * the input by reference and returns a **dynamically allocated**
 * int array containing the inputs. Returns NULL on invalid input
 */
int *read_input(char *filename, int *size);
