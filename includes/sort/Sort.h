#pragma once

#include "core/App.h"

#if defined(TESTING)
#define HIGH_RES_WAIT(t) do {} while(0);
#else 
#define HIGH_RES_WAIT(t)\
    auto start = std::chrono::high_resolution_clock::now();\
    while (true) {\
        auto now = std::chrono::high_resolution_clock::now();\
        double elapsed = std::chrono::duration<double, std::milli>(now - start).count();\
        if (elapsed >= (t)) break;\
    }
#endif
class Sort
{
protected:
    std::atomic<size_t> m_first;
    std::atomic<size_t> m_second;

public:
    std::vector<int>& elems;

    std::atomic<bool> sorted;
    std::atomic<bool> isSorting;
    std::atomic<bool> isShuffling;
    std::atomic<bool> wantClose;
    std::atomic<bool> wantStop;

    static float speed;
    static int length;

    double startTime = 0.0;
    double lastTime = 0.0;

    unsigned int swaps; 
    unsigned int comparisions; 

    size_t getFirst() noexcept { return m_first.load(); }
    size_t getSecond() noexcept { return m_second.load(); }

    Sort(std::vector<int>& t_arr);

    void swap(std::vector<int>& array, size_t a, size_t b);

    void shuffle();
    void reverse();
    void setLength(unsigned int length);
    
    virtual void sort() = 0;
};