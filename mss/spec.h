typedef struct _mss_result {
	char filename[100];
	int algo_index;
	int input_size;
	int index_a;
	int index_b;
	int sum;
	int running_time;
} mss_result;

/*
 * A function that takes inputs for the mss problem, and returns
 * a **dynamically allocated** mss_result containing the answers.
 * Returns NULL on invalid input.
 */
typedef mss_result *mss_function(int *data, int size);

#define DEBUG_EXPR "line %d of \"%s\" function <%s>\n",__LINE__,__FILE__,__func__
