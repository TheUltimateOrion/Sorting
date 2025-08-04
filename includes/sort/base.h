#pragma once

#include <atomic>
#include <vector>
#include <memory>

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
        void generateArray(size_t t_size);
    protected:
        std::atomic<size_t> m_first;
        std::atomic<size_t> m_second;
    public:
        virtual ~BaseSort() = default;
        
        std::vector<int> elems;

        using item_t = decltype(elems)::value_type;

        std::atomic<bool> sorted;
        std::atomic<bool> isSorting;
        std::atomic<bool> isShuffling;
        std::atomic<bool> isChecking;
        std::atomic<bool> wantClose;
        std::atomic<bool> wantStop;
        std::atomic<bool> running;

        static float s_speed;

        std::mutex mutex;

        Core::Timer timer;
        Core::Timer realTimer;

        uint64_t swaps{0}; 
        uint64_t comparisons{0}; 

        size_t getFirst() noexcept { return m_first.load(); }
        size_t getSecond() noexcept { return m_second.load(); }

        BaseSort();

        void swap(std::vector<item_t>& array, size_t a, size_t b);

        void shuffle();
        void reverse();
        void check();
        void setLength(size_t length);
        
        virtual void sort() = 0;
    };
} // namespace Sort
