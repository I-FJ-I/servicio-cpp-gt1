#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <vector>

#include "algorithms/mergesort.hpp"

using bench::algorithms::merge_sort;

namespace {

// Comprueba que merge_sort ordena `input` y conserva exactamente sus elementos
// (mismo resultado que std::sort sobre una copia).
void expect_sorted_permutation(std::vector<int> input) {
  std::vector<int> expected = input;
  std::sort(expected.begin(), expected.end());

  merge_sort(input);

  EXPECT_TRUE(std::is_sorted(input.begin(), input.end()));
  EXPECT_EQ(input, expected);
}

std::vector<int> random_vector(std::size_t size, unsigned seed) {
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> distribution(-1'000'000, 1'000'000);
  std::vector<int> values(size);
  for (int& value : values) value = distribution(generator);
  return values;
}

}  // namespace

TEST(MergeSort, EmptyVectorStaysEmpty) {
  std::vector<int> data;
  merge_sort(data);
  EXPECT_TRUE(data.empty());
}

TEST(MergeSort, SingleElementIsUnchanged) {
  std::vector<int> data{42};
  merge_sort(data);
  EXPECT_EQ(data, std::vector<int>{42});
}

TEST(MergeSort, AlreadySortedInputIsUnchanged) {
  expect_sorted_permutation({1, 2, 3, 4, 5, 6, 7, 8});
}

TEST(MergeSort, ReverseSortedInputIsSorted) {
  expect_sorted_permutation({9, 8, 7, 6, 5, 4, 3, 2, 1, 0});
}

TEST(MergeSort, KeepsAllDuplicates) {
  expect_sorted_permutation({5, 1, 5, 3, 1, 5, 3, 3, 1});
}

TEST(MergeSort, HandlesNegativeNumbers) {
  expect_sorted_permutation({3, -1, 0, -7, 12, -1, 5});
}

TEST(MergeSort, SortsOddAndEvenSizes) {
  expect_sorted_permutation(random_vector(7, 1));
  expect_sorted_permutation(random_vector(8, 2));
}

TEST(MergeSort, SortsLargeRandomInput) {
  expect_sorted_permutation(random_vector(100'000, 12345));
}
