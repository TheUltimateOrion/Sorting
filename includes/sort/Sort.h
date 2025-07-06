#pragma once
#include "../App.h"

static float last_time;

class Sort
{
public:
    bool sorted;
    bool isSorting;
    bool isShuffling;
    bool wantClose;
    bool wantStop;
    float speed;
    float start_time;
    unsigned int swaps = 0; 
    unsigned int comparisions = 0; 

    std::vector<int> elems;

    Sort(std::vector<int>& arr);

    void swap(std::vector<int>& array, int a, int b);

    void shuffle();
    void reverse();
    void setSpeed(float speed);
    void setLength(unsigned int length);
    
    virtual void sort() = 0;
};
