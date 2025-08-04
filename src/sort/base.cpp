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

    void BaseSort::generateArray(size_t t_size)
    {
        LOGINFO("Generating array");
        elems.resize(t_size);
        for (size_t i = 0; i < t_size; ++i)
        {
            elems[i] = static_cast<item_t>(i + 1);
        }
    }

    BaseSort::BaseSort() : m_first(0), m_second(0), sorted(true), isSorting(false), isShuffling(false), isChecking(false), wantClose(false), wantStop(false), running(false) 
    {
        constexpr int defaultSize = 512;
        generateArray(defaultSize);
    }

    void BaseSort::reverse()
    {
        LOGINFO("Reversing");

        isShuffling = true;
        wantStop = false;

        elems.resetCounters();

        SortArray temp {elems};


        std::reverse(temp.begin(), temp.end());
        for (size_t i = 0; i < temp.size(); i++)
        {
            elems[i] = temp[i];

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
        
        elems.resetCounters();
        
        SortArray temp {elems};

        std::shuffle(temp.begin(), temp.end(), std::random_device{});

        for (size_t i = 0; i < temp.size(); ++i)
        {
            elems[i] = temp[i];

            m_first = i;
            m_second = m_first.load();
            
            
            Core::Timer::sleep(1000.0 / static_cast<double>(elems.size()), realTimer);
            if (wantClose || wantStop) return;
        }

        sorted = false;
        isShuffling = false;

        LOGINFO("Shuffling Done");
        std::this_thread::sleep_for(500ms);
    }

    void BaseSort::check()
    {   
        wantStop = false;
        isChecking = true;

        SortArray temp {elems};

        realTimer.end();
        timer.end();

        for (size_t i = 0; i < temp.size(); ++i)
        {
            if (temp[i] != static_cast<item_t>(i + 1)) 
            {
                break;
            }
            
            m_first = i;
            m_second = m_first.load();
            
            Core::Timer::sleep(500.0 / static_cast<double>(temp.size()), realTimer);
            if (wantClose || wantStop) return;
        }
        
        isChecking = false;
        
        LOGINFO("Check completed");
    }

    void BaseSort::swap(SortArray<item_t>& array, size_t a, size_t b)
    {
        m_first = a;
        m_second = b;

        array.swap(a, b);
        
        Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
    }

    void BaseSort::setLength(size_t len)
    {
        LOGINFO("Resizing to " << len);
        generateArray(len);
    }
} // namespace Sort
