#pragma once

#include <atomic>
#include <vector>

namespace Sort 
{
    class BaseSort
    {
    private:
        bool m_isRadix = false;
    protected:
        std::atomic<size_t> m_first;
        std::atomic<size_t> m_second;

    public:
        std::vector<int>& elems;

        std::atomic<bool> sorted;
        std::atomic<bool> isSorting;
        std::atomic<bool> isShuffling;
        std::atomic<bool> isChecking;
        std::atomic<bool> wantClose;
        std::atomic<bool> wantStop;

        static float s_speed;
        static int s_length;

        double startTime = 0.0;
        double lastTime = 0.0;

        unsigned int swaps = 0; 
        unsigned int comparisons = 0; 

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
