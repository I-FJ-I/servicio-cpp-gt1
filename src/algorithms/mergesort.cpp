#include "algorithms/mergesort.hpp"

#include <cstddef>

namespace bench::algorithms {
namespace {

// Fusiona los tramos ordenados data[lo, mid) y data[mid, hi) usando `buffer`.
void merge(std::vector<int>& data, std::vector<int>& buffer, std::size_t lo,
           std::size_t mid, std::size_t hi) {
  std::size_t left = lo;
  std::size_t right = mid;
  std::size_t out = lo;

  while (left < mid && right < hi) {
    // `<=` mantiene la estabilidad: a igualdad, primero el de la izquierda
    if (data[left] <= data[right]) {
      buffer[out++] = data[left++];
    } else {
      buffer[out++] = data[right++];
    }
  }
  while (left < mid) buffer[out++] = data[left++];
  while (right < hi) buffer[out++] = data[right++];

  for (std::size_t i = lo; i < hi; ++i) data[i] = buffer[i];
}

void sort_range(std::vector<int>& data, std::vector<int>& buffer,
                std::size_t lo, std::size_t hi) {
  if (hi - lo < 2) return;
  const std::size_t mid = lo + (hi - lo) / 2;
  sort_range(data, buffer, lo, mid);
  sort_range(data, buffer, mid, hi);
  merge(data, buffer, lo, mid, hi);
}

}  // namespace

void merge_sort(std::vector<int>& data) {
  if (data.size() < 2) return;
  // Un único buffer auxiliar reservado una vez: evita una reserva por llamada
  std::vector<int> buffer(data.size());
  sort_range(data, buffer, 0, data.size());
}

}  // namespace bench::algorithms
