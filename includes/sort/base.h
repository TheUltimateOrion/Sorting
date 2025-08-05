#pragma once

#include <atomic>
#include <memory>

#include "array.h"
#include "core/timer.h"

namespace Core
{
    class App;
}

namespace Sort
{
    class BaseSort
    {
    private:
        void generateArray(std::size_t t_size);

    protected:
        std::atomic<std::size_t> m_first {0};
        std::atomic<std::size_t> m_second {0};

    public:
        SortArray<int> elems {};
        SortArray<int> auxillary {};

        using elem_t = typename decltype(elems)::value_type;

        Core::Timer       timer {};
        Core::Timer       realTimer {};

        std::atomic<bool> sorted {true};
        std::atomic<bool> isSorting {false};
        std::atomic<bool> isShuffling {false};
        std::atomic<bool> isChecking {false};
        std::atomic<bool> wantClose {false};
        std::atomic<bool> wantStop {false};
        std::atomic<bool> running {false};
        std::atomic<bool> hasRadix {false};

        static float      s_speed;

        std::size_t       getFirst() noexcept { return m_first.load(); }

        std::size_t       getSecond() noexcept { return m_second.load(); }

        BaseSort();

        void         swap(SortArray<elem_t>& array, std::size_t a, std::size_t b);

        void         shuffle();
        void         reverse();
        void         check();
        void         setLength(std::size_t length);

        virtual void sort() = 0;
        virtual ~BaseSort() = default;
    };
}  // namespace Sort
