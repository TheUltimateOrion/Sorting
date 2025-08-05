#include "core/registry/sort_registry.h"
#include <gtest/gtest.h>

class SortRegistryTest : public ::testing::TestWithParam<char const*>
{
public:
    Core::SortRegistry reg;
};

INSTANTIATE_TEST_SUITE_P(
    AllSorts,
    SortRegistryTest,
    ::testing::Values(
        "bubble",
        "quick",
        "merge",
        "insertion",
        "selection",
        "comb",
        "gravity",
        "pigeon_hole",
        "bogo",
        "radix_lsd"
    )
);

TEST_P(SortRegistryTest, HandlesRandomInput)
{
    reg.registerAllSorts();
    std::vector<int> input    = {5, 3, 8, 1, 9, 2, 4, 7, 6};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    char const*      id       = GetParam();
    auto*            entry    = reg.get(id);
    ASSERT_NE(entry, nullptr);
    auto sorter = entry->factory();
    sorter->sort();
    ASSERT_EQ(input, expected);
}

TEST_P(SortRegistryTest, HandlesReversedInput)
{
    reg.registerAllSorts();
    std::vector<int> input    = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    char const*      id       = GetParam();
    auto*            entry    = reg.get(id);
    ASSERT_NE(entry, nullptr);
    auto sorter = entry->factory();
    sorter->sort();
    ASSERT_EQ(input, expected);
}

TEST_P(SortRegistryTest, HandlesEmptyInput)
{
    reg.registerAllSorts();
    std::vector<int> input = {};
    char const*      id    = GetParam();
    auto*            entry = reg.get(id);
    ASSERT_NE(entry, nullptr);
    auto sorter = entry->factory();
    sorter->sort();
    ASSERT_TRUE(input.empty());
}

TEST_P(SortRegistryTest, HandlesSingleElement)
{
    reg.registerAllSorts();
    std::vector<int> input = {42};
    char const*      id    = GetParam();
    auto*            entry = reg.get(id);
    ASSERT_NE(entry, nullptr);
    auto sorter = entry->factory();
    sorter->sort();
    ASSERT_EQ(input, std::vector<int>({42}));
}
