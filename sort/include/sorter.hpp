#pragma once
#include <vector>
#include "data.hpp"

typedef void (*sort_func)(Data *data);

bool sorts_correctly(Data *data, sort_func sort);

sort_func std_sort;
sort_func null_sort;  // doesn't do anything
sort_func merge_sort;
sort_func quick_sort;
sort_func ho_sort;
