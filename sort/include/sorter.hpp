#pragma once
#include <vector>
#include "data.hpp"

typedef void (*sort_func)(int *array, int begin, int end);

bool validate_sort(sort_func sorter, bool verbose=false, int iterations=5000);

void std_sort(int *array, int begin, int end);
void null_sort(int *array, int begin, int end);
void insertion_sort(int *array, int begin, int end);
void merge_sort(int *array, int begin, int end);
void quick_sort(int *array, int begin, int end);
void ho_sort(int *array, int begin, int end);
