#pragma once
#include "../App.h"

#define HIGH_RES_WAIT(t)\
    auto start = std::chrono::high_resolution_clock::now();\
    while (true) {\
        auto now = std::chrono::high_resolution_clock::now();\
        double elapsed = std::chrono::duration<double, std::milli>(now - start).count();\
        if (elapsed >= (t)) break;\
    }
class Sort
{
public:
    std::atomic<bool> sorted;
    std::atomic<bool> isSorting;
    std::atomic<bool> isShuffling;
    std::atomic<bool> wantClose;
    std::atomic<bool> wantStop;

    static float speed;
    float start_time = 0.0f;
    float last_time = 0.0f;
    unsigned int swaps = 0; 
    unsigned int comparisions = 0; 

    int first = 0;
    int second = 0;

    std::vector<int>& elems;

    Sort(std::vector<int>& arr);

    void swap(std::vector<int>& array, int a, int b);

    void shuffle();
    void reverse();
    void setLength(unsigned int length);
    
    virtual void sort() = 0;
};