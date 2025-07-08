#include "renderer/Renderer.h"

float Sort::speed = 1.0f;

Sort::Sort(std::vector<int>& arr) : elems(arr), isSorting(false), isShuffling(false), sorted(true), wantClose(false), wantStop(false), start_time(0) {}

void Sort::reverse()
{
    LOGINFO("Reversing");

    this->isShuffling = true;
    this->wantStop = false;
    this->swaps = 0;
    this->comparisions = 0;

    std::vector<int> temp(elems.size());
    for (int i = 0; i < elems.size(); i++)
        temp[i] = elems[i];

    std::reverse(temp.begin(), temp.end());
    for (int i = 0; i < temp.capacity(); i++)
    {
        elems[i] = temp[i];
        this->first = i;
        this->second = this->first;
        if (wantClose || wantStop) return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    sorted = false;
    this->isShuffling = false;
    this->start_time = static_cast<float>(clock()) / 1000.0f;
    this->last_time = this->start_time;
}

void Sort::shuffle()
{
    LOGINFO("Shuffling");

    this->isShuffling = true;
    this->wantStop = false;
    this->swaps = 0;
    this->comparisions = 0;

    std::vector<int> temp(elems.size());

    for (int i = 0; i < elems.size(); i++)
        temp[i] = elems[i];

    std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(0));

    for (int i = 0; i < temp.capacity(); i++)
    {
        elems[i] = temp[i];
        this->first = i;
        this->second = this->first;
        
        HIGH_RES_WAIT(1000.0 / static_cast<double>(elems.size()))
        if (wantClose || wantStop) return;
    }

    sorted = false;
    isShuffling = false;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    this->start_time = static_cast<float>(clock()) / 1000.0f;
    this->last_time = this->start_time;
}

void Sort::swap(std::vector<int>& array, int a, int b)
{
    this->first = a;
    this->second = b;
    std::swap(array[a], array[b]);
    
    HIGH_RES_WAIT(1.f / Sort::speed);

    swaps++;
}

void Sort::setLength(unsigned int len)
{
    LOGINFO("Resizing to " << len);
    elems.resize(len);
    for (int index = 0; index < elems.size(); index++)
        elems[index] = index + 1;
}