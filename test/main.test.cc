#include <gtest/gtest.h>

#include "sort/sort.h"

// --- Shared Test Fixture for All Sorts ---
template <typename SortType>
class SortTest : public ::testing::Test {};

using SortImplementations = ::testing::Types<
    BubbleSort, QuickSort, MergeSort, InsertionSort,
    SelectionSort, CombSort, GravitySort, PigeonHoleSort, BogoSort
>;

TYPED_TEST_SUITE(SortTest, SortImplementations);

TYPED_TEST(SortTest, HandlesRandomInput) {
    std::vector<int> input = {5, 3, 8, 1, 9, 2, 4, 7, 6};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    TypeParam sorter(input);
    sorter.sort();
    ASSERT_EQ(input, expected);
}

TYPED_TEST(SortTest, HandlesReversedInput) {
    std::vector<int> input = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    TypeParam sorter(input);
    sorter.sort();
    ASSERT_EQ(input, expected);
}

TYPED_TEST(SortTest, HandlesEmptyInput) {
    std::vector<int> input = {};
    TypeParam sorter(input);
    sorter.sort();
    ASSERT_TRUE(input.empty());
}

TYPED_TEST(SortTest, HandlesSingleElement) {
    std::vector<int> input = {42};
    TypeParam sorter(input);
    sorter.sort();
    ASSERT_EQ(input, std::vector<int>({42}));
}

// --- RadixLSDSort Special Tests ---
TEST(RadixSortTest, HandlesRandomInput) {
    std::vector<int> input = {5, 3, 8, 1, 9, 2, 4, 7, 6};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    RadixLSDSort sorter(input, 10);
    sorter.sort();
    ASSERT_EQ(input, expected);
}

TEST(RadixSortTest, HandlesReversedInput) {
    std::vector<int> input = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    RadixLSDSort sorter(input, 10);
    sorter.sort();
    ASSERT_EQ(input, expected);
}

TEST(RadixSortTest, HandlesEmptyInput) {
    std::vector<int> input = {};
    RadixLSDSort sorter(input, 10);
    sorter.sort();
    ASSERT_TRUE(input.empty());
}

TEST(RadixSortTest, HandlesSingleElement) {
    std::vector<int> input = {42};
    RadixLSDSort sorter(input, 10);
    sorter.sort();
    ASSERT_EQ(input, std::vector<int>({42}));
}