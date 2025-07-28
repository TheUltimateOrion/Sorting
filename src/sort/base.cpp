#include "sort/base.h"

#include <thread>
#include <random>
#include <algorithm>

#ifndef TESTING
#include "core/app.h"
#include "renderer/sort_view.h"
#endif

#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "utils/common.h"


using namespace std::literals::chrono_literals;

namespace Sort 
{
    float BaseSort::s_speed = 1.0f;
    int BaseSort::s_length = 512;

    BaseSort::BaseSort(std::vector<int>& t_arr, bool t_isRadix) : m_isRadix(t_isRadix), m_first(0), m_second(0), elems(t_arr), sorted(true), isSorting(false), isShuffling(false), isChecking(false), wantClose(false), wantStop(false), running(false) {}

    void BaseSort::reverse()
    {
        LOGINFO("Reversing");

        isShuffling = true;
        wantStop = false;
        swaps = 0;
        comparisons = 0;

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

        sorted = false;
        isShuffling = false;

        std::this_thread::sleep_for(500ms);
    }

    void BaseSort::shuffle()
    {
        LOGINFO("Shuffling");

        isShuffling = true;
        wantStop = false;
        swaps = 0;
        comparisons = 0;

        std::vector<int> temp(elems.size());

        {
            LOCK_GUARD;
            temp = elems; // Copy the original elements to temp
        }

        std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(0));

        for (size_t i = 0; i < temp.capacity(); ++i)
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
    }

    void BaseSort::check()
    {   
        wantStop = false;
        isChecking = true;

        std::vector<int> temp(elems.size());

        {
            LOCK_GUARD;
            temp = elems; // Copy the original elements to temp
        }

        realTimer.end();
        timer.end();

        for (size_t i = 0; i < temp.capacity(); ++i)
        {
            if (temp[i] != static_cast<int>(i) + 1) 
            {
                break;
            }
            
            m_first = i;
            m_second = m_first.load();
            
            HIGH_RES_WAIT(500.0 / static_cast<double>(temp.size()))
            if (wantClose || wantStop) return;
        }
        

        isChecking = false;
        LOGINFO("Check completed");
    }

    void BaseSort::swap(std::vector<int>& array, size_t a, size_t b)
    {
        m_first = a;
        m_second = b;

        {
            LOCK_GUARD;
            if (a >= array.size() || b >= array.size()) 
            {
                LOGERR("Swap indices out of bounds: " << a << ", " << b);
                return;
            }

            std::swap(array[a], array[b]);
        }
        
        HIGH_RES_WAIT(1.f / BaseSort::s_speed);

        swaps++;
    }

    void BaseSort::setLength(unsigned int len)
    {
        LOGINFO("Resizing to " << len);
        elems.resize(len);
        
        for (size_t i = 0; i < elems.size(); ++i)
        {
            elems[i] = i + 1;
        }
    }
} // namespace Sort
