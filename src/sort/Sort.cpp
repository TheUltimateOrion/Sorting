#include "sort/Sort.h"

#ifndef TESTING
#include "renderer/Renderer.h"
#endif

float Sort::speed = 1.0f;
int Sort::length = 512;

Sort::Sort(std::vector<int>& arr) : elems(arr), isSorting(false), isShuffling(false), sorted(true), wantClose(false), wantStop(false), start_time(0), first(0), second(0) {}

void Sort::reverse()
{
    LOGINFO("Reversing");

    this->isShuffling = true;
    this->wantStop = false;
    this->swaps = 0;
    this->comparisions = 0;

    std::vector<int> temp(elems.size());

    {
        LOCK_GUARD;
        temp = elems; // Copy the original elements to temp
    }

    std::reverse(temp.begin(), temp.end());
    for (int i = 0; i < temp.capacity(); i++)
    {
        {
            LOCK_GUARD;
            elems[i] = temp[i];
        }

        this->first = i;
        this->second = this->first.load();
        if (wantClose || wantStop) return;
    }

    std::this_thread::sleep_for(500ms);

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

    {
        LOCK_GUARD;
        temp = elems; // Copy the original elements to temp
    }

    std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(0));

    for (int i = 0; i < temp.capacity(); i++)
    {
        {
            LOCK_GUARD;
            elems[i] = temp[i];
        }
        this->first = i;
        this->second = this->first.load();
        
        HIGH_RES_WAIT(1000.0 / static_cast<double>(elems.size()))
        if (wantClose || wantStop) return;
    }

    sorted = false;
    isShuffling = false;

    std::this_thread::sleep_for(500ms);
    this->start_time = static_cast<float>(clock()) / 1000.0f;
    this->last_time = this->start_time;
}

void Sort::swap(std::vector<int>& array, int a, int b)
{
    this->first = a;
    this->second = b;

    {
        LOCK_GUARD;
        if (a < 0 || a >= array.size() || b < 0 || b >= array.size()) {
            LOGERR("Swap indices out of bounds: " << a << ", " << b);
            return;
        }
        std::swap(array[a], array[b]);
    }
    
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