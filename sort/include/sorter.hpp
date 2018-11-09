#pragma once
#include <vector>
#include "data.hpp"

typedef void (*sort_func)(Data &data);

bool validate_sort(sort_func sort, bool verbose=false, int iterations=5000);

void std_sort(Data &data);
void null_sort(Data &data);
void merge_sort(Data &data);
void quick_sort(Data &data);
void ho_sort(Data &data);
