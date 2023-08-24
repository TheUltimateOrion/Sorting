#pragma once
#include "../main.h"

static float last_time;

class Sort
{
public:
    bool sorted;
    bool isSorting;
    bool wantBreak;
    ImGuiIO& io;
    std::vector<int> elems;
    float speed;
    float start_time;

    Sort(std::vector<int>& arr, ImGuiIO& io);

    void swap(std::vector<int>& array, int a, int b);

    void shuffle();
    void setSpeed(float speed);
    virtual void sort() = 0;
};
