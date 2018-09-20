/*
 * Generates input for mss with the specified size.
 * Returns a **dynamically allocated** int array containing
 * the inputs
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
