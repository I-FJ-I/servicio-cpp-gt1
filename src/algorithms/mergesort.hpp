#pragma once

#include <vector>

namespace bench::algorithms {

// Ordena `data` de menor a mayor con MergeSort (estable, O(n log n)).
void merge_sort(std::vector<int>& data);

}  // namespace bench::algorithms
