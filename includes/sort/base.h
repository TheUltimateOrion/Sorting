#pragma once

#include <atomic>
#include <memory>

#include "array.h"
#include "core/timer.h"
#include "flags.h"

#define RETURN_IF_STOPPED(ret)                                               \
    if (m_flags.wantClose.load() || m_flags.wantStop.load()) { return ret; }

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

        Flags                    m_flags;

    public:
        SortArray<int> elems {};
        using elem_t = typename decltype(elems)::value_type;

        SortArray<elem_t> auxillary {};

        Core::Timer       timer {};
        Core::Timer       realTimer {};

        static float      s_speed;

        std::size_t       getFirst() noexcept { return m_first.load(); }

        std::size_t       getSecond() noexcept { return m_second.load(); }

        BaseSort();

        void         swap(SortArray<elem_t>& array, std::size_t a, std::size_t b);

        void         shuffle();
        void         reverse();
        void         check();
        void         setLength(std::size_t length);

        Flags&       getFlags() { return m_flags; }

        virtual void sort() = 0;
        virtual ~BaseSort() = default;
    };
}  // namespace Sort
