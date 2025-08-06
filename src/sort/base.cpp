#include "sort/base.h"

#include <algorithm>
#include <random>
#include <thread>

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
    float BaseSort::s_speed {1.0f};

    void  BaseSort::generateArray(std::size_t t_size)
    {
        LOGINFO("Generating array");
        elems.resize(t_size);
        for (std::size_t i = 0; i < t_size; ++i) { elems[i] = static_cast<elem_t>(i + 1); }
    }

    BaseSort::BaseSort()
    {
        constexpr std::size_t defaultSize = 512;
        generateArray(defaultSize);
    }

    void BaseSort::reverse()
    {
        m_flags.isShuffling = true;
        m_flags.wantStop    = false;

        elems.resetCounters();

        auxillary = elems;
        auxillary.resetCounters();

        std::reverse(auxillary.begin(), auxillary.end());
        for (std::size_t i = 0; i < auxillary.size(); ++i)
        {
            elems[i] = auxillary[i];

            m_first  = i;
            m_second = m_first.load();

            Core::Timer::sleep(1000.0 / static_cast<double>(elems.size()), realTimer);
        }

        m_flags.hasSorted   = false;
        m_flags.isShuffling = false;

        std::this_thread::sleep_for(500ms);
    }

    void BaseSort::shuffle()
    {
        m_flags.isShuffling = true;
        m_flags.wantStop    = false;

        elems.resetCounters();

        auxillary = elems;
        auxillary.resetCounters();

        std::shuffle(auxillary.begin(), auxillary.end(), std::random_device {});

        for (std::size_t i = 0; i < auxillary.size(); ++i)
        {
            elems[i] = auxillary[i];

            m_first  = i;
            m_second = m_first.load();

            Core::Timer::sleep(1000.0 / static_cast<double>(elems.size()), realTimer);
            RETURN_IF_STOPPED();
        }

        m_flags.hasSorted   = false;
        m_flags.isShuffling = false;

        LOGINFO("Shuffling Done");
        std::this_thread::sleep_for(500ms);
    }

    void BaseSort::check()
    {
        m_flags.isChecking = true;
        m_flags.wantStop   = false;

        auxillary          = elems;

        realTimer.end();
        timer.end();

        for (std::size_t i = 0; i < auxillary.size(); ++i)
        {
            if (auxillary[i] != static_cast<elem_t>(i + 1)) { break; }

            m_first  = i;
            m_second = m_first.load();

            Core::Timer::sleep(500.0 / static_cast<double>(auxillary.size()), realTimer);
            RETURN_IF_STOPPED();
        }

        m_flags.isChecking = false;

        LOGINFO("Check completed");
    }

    void BaseSort::swap(SortArray<elem_t>& array, std::size_t a, std::size_t b)
    {
        m_first  = a;
        m_second = b;

        array.swap(a, b);

        Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
    }

    void BaseSort::setLength(std::size_t len)
    {
        LOGINFO("Resizing to " << len);
        generateArray(len);
    }
}  // namespace Sort
