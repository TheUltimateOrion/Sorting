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
        bool m_isRadix = false;
        std::weak_ptr<Core::App> m_app;
    protected:
        std::atomic<size_t> m_first;
        std::atomic<size_t> m_second;

    public:
        virtual ~BaseSort() = default;
        
        std::vector<int>& elems;

        std::atomic<bool> sorted;
        std::atomic<bool> isSorting;
        std::atomic<bool> isShuffling;
        std::atomic<bool> isChecking;
        std::atomic<bool> wantClose;
        std::atomic<bool> wantStop;
        std::atomic<bool> running;

        static float s_speed;
        static int s_length;

        static std::mutex s_mutex;

        Core::Timer timer;
        Core::Timer realTimer;

        uint64_t swaps{0}; 
        uint64_t comparisons{0}; 

        size_t getFirst() noexcept { return m_first.load(); }
        size_t getSecond() noexcept { return m_second.load(); }

        bool isRadix() const noexcept { return m_isRadix; }

        BaseSort(std::vector<int>& t_arr, bool t_isRadix = false);

        void swap(std::vector<int>& array, size_t a, size_t b);

        void shuffle();
        void reverse();
        void check();
        void setLength(unsigned int length);
        
        virtual void sort() = 0;
    };
} // namespace Sort
