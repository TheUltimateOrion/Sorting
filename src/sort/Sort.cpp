#include "sort/Sort.h"

Sort::Sort(std::vector<int>& arr, ImGuiIO& io) : elems(arr), sorted(false), wantBreak(false), speed(speed), io(io), last_time((float)clock() / 1000.0f) {}

void Sort::shuffle()
{
    std::shuffle(std::begin(this->elems), std::end(this->elems), std::default_random_engine(0));
}

void Sort::swap(std::vector<int>& array, int a, int b)
{
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}