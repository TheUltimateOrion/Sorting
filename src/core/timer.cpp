#include "core/timer.h"

#include <iostream>

namespace Core
{
    void Timer::start() noexcept
    {
        m_start.reset();
        m_start = std::chrono::high_resolution_clock::now();
    }

    void Timer::end() noexcept
    {
        m_end.reset();
        m_end = std::chrono::high_resolution_clock::now();
        m_duration = m_end.value() - m_start.value();
    }

    double Timer::getCurrentDuration() const
    {
        if (!m_start.has_value()) 
        {
            return 0.0;
        } 
        else if (!m_end.has_value())
        {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double>(now - m_start.value()).count();
        }
        else
        {
            return m_duration.count();
        }

    }
} // namespace Core
