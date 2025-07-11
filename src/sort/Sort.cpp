#include "sort/Sort.h"
#include "core/logging/Logging.h"

#ifndef TESTING
#include "renderer/Renderer.h"
#endif

float Sort::speed = 1.0f;
int Sort::length = 512;

Sort::Sort(std::vector<int>& t_arr) : m_first(0), m_second(0), elems(t_arr), sorted(true), isSorting(false), isShuffling(false), wantClose(false), wantStop(false), startTime(0) {}

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
    for (size_t i = 0; i < temp.capacity(); i++)
    {
        {
            LOCK_GUARD;
            elems[i] = temp[i];
        }

        m_first = i;
        m_second = m_first.load();
        if (wantClose || wantStop) return;
    }

    std::this_thread::sleep_for(500ms);

    sorted = false;
    this->isShuffling = false;
    this->startTime = AppCtx::getTimestamp();
    this->lastTime = this->lastTime;
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

    for (size_t i = 0; i < temp.capacity(); i++)
    {
        {
            LOCK_GUARD;
            elems[i] = temp[i];
        }
        m_first = i;
        m_second = m_first.load();
        
        HIGH_RES_WAIT(1000.0 / static_cast<double>(elems.size()))
        if (wantClose || wantStop) return;
    }

    sorted = false;
    isShuffling = false;

    std::this_thread::sleep_for(500ms);
    this->startTime = AppCtx::getTimestamp();
    this->lastTime = this->startTime;
}

void Sort::swap(std::vector<int>& array, size_t a, size_t b)
{
    m_first = a;
    m_second = b;

    {
        LOCK_GUARD;
        if (a >= array.size() || b >= array.size()) {
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
    for (size_t i = 0; i < elems.size(); ++i)
        elems[i] = i + 1;
}