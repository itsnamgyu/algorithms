#pragma once
#include <vector>
#include "data.hpp"

typedef void (*sort_func)(int *array, int begin, int end);

bool validate_sort(sort_func sorter, bool verbose=false, int iterations=2000);

void std_sort(int *array, int begin, int end);
void null_sort(int *array, int begin, int end);
void insertion_sort(int *array, int begin, int end);
void merge_sort(int *array, int begin, int end);
void heap_sort(int *array, int begin, int end);
void quick_sort(int *array, int begin, int end);

void median_qs(int *array, int begin, int end);

void sorty_mcsortface(int *array, int begin, int end);
