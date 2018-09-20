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
 * As the name suggests.
 */
void generate_input_file(char *filename, int size);
