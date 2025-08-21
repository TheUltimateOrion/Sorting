#include "sort/base.h"

#include "core/logging/logging.h"
#include "renderer/context.h"
#include "utils/common.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>

using namespace std::literals::chrono_literals;

namespace Sort
{
    float BaseSort::s_speed {1.0f};

    void  BaseSort::generateArray(std::size_t t_size)
    {
        if (t_size != elems.size()) { elems.resize(t_size); }

        for (std::size_t i = 0; i < t_size; ++i) { elems[i] = static_cast<elem_t>(i + 1); }
    }

    BaseSort::BaseSort()
    {
        constexpr std::size_t defaultSize = 512;
        generateArray(defaultSize);
    }

    void BaseSort::shuffle(bool t_isReversed)
    {
        m_flags.setFlags(FlagGroup::StartShuffling);

        elems.resetCounters();

        auxillary = elems;
        auxillary.resetCounters();

        if (t_isReversed)
        {
            LOG_INFO("Reversing");
            std::reverse(auxillary.begin(), auxillary.end());
        }
        else
        {
            LOG_INFO("Shuffling");
            std::shuffle(auxillary.begin(), auxillary.end(), std::random_device {});
        }

        for (std::size_t i = 0; i < auxillary.size(); ++i)
        {
            elems[i] = auxillary[i];

            m_first  = i;
            m_second = m_first.load();

            Core::Timer::Sleep(1000.0 / static_cast<double>(elems.size()));
            RETURN_IF_STOPPED();
        }

        LOG_INFO("Shuffling Done");

        m_flags.setFlags(FlagGroup::DoneShuffling);
        std::this_thread::sleep_for(500ms);
    }

    void BaseSort::check()
    {
        m_flags.setFlags(FlagGroup::StartChecking);

        auxillary = elems;

        realTimer.end();
        timer.end();

        LOG_INFO("Checking");

        for (std::size_t i = 0; i < auxillary.size(); ++i)
        {
            if (auxillary[i] != static_cast<elem_t>(i + 1)) { break; }

            m_first  = i;
            m_second = m_first.load();

            Core::Timer::Sleep(500.0 / static_cast<double>(auxillary.size()));
            RETURN_IF_STOPPED();
        }

        LOG_INFO("Check completed");

        m_flags.setFlags(FlagGroup::DoneChecking);
    }

    void BaseSort::swap(SortArray<elem_t>& array, std::size_t a, std::size_t b)
    {
        m_first  = a;
        m_second = b;

        array.swap(a, b);

        Core::Timer::Sleep(1.f / BaseSort::s_speed, realTimer);
    }

    void BaseSort::setLength(std::size_t len)
    {
        LOG_INFO("Resizing to " << len);
        generateArray(len);
    }

    void BaseSort::reset() noexcept
    {
        generateArray(elems.size());
        m_first = m_second = 0;
        m_flags.reset();
        realTimer.reset();
        timer.reset();
    }

}  // namespace Sort
