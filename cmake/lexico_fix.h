#pragma once

#include <string>
#include <compare>
#include <algorithm>

namespace std {
    template <typename Iter1, typename Iter2>
    std::weak_ordering lexicographical_compare_three_way(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2) {
        if (std::lexicographical_compare(first1, last1, first2, last2))
            return std::weak_ordering::less;
        if (std::lexicographical_compare(first2, last2, first1, last1))
            return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }
}