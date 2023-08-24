#include "sort/Sort.h"


Sort::Sort(std::vector<int>& arr, ImGuiIO& io) : elems(arr), isSorting(false), sorted(true), wantBreak(false), speed(speed), io(io), start_time(0) {}

void Sort::shuffle()
{
    std::shuffle(std::begin(this->elems), std::end(this->elems), std::default_random_engine(0));
    sorted = false;
    this->start_time = (float)clock() / 1000.0f;
    ::last_time = this->start_time;
}

void Sort::setSpeed(float speed)
{
    this->speed = speed;
}

void Sort::swap(std::vector<int>& array, int a, int b)
{
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}
