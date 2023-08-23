#pragma once
#include "../main.h"

class Sort
{
public:
    bool sorted;
    bool wantBreak;
    ImGuiIO& io;
    std::vector<int> elems;
    int speed;
    float last_time;
    
    Sort(std::vector<int>& arr, ImGuiIO& io);

    void swap(std::vector<int>& array, int a, int b);

    void shuffle();
    virtual void sort(float speed = 1) = 0;
};