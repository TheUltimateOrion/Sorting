#include "core/timer.h"

#include <iostream>

#include "core/logging/logging.h"

namespace Core
{
    void Timer::start() noexcept
    {
        std::scoped_lock<std::mutex> lock {m_mutex};

        m_start = std::chrono::high_resolution_clock::now();

        m_end.reset();
        m_pauseStart.reset();

        m_pauseDuration = std::chrono::duration<double>::zero();

        m_paused        = false;
        m_ended         = false;
    }

    void Timer::end() noexcept
    {
        std::scoped_lock<std::mutex> lock {m_mutex};

        if (!m_start.has_value())
        {
            return;
        }

        // If paused, include last pause in total pause duration
        if (m_paused && m_pauseStart.has_value())
        {
            m_pauseDuration += std::chrono::high_resolution_clock::now() - m_pauseStart.value();
            m_paused = false;
            m_pauseStart.reset();
        }

        if (!m_ended)
        {
            m_end   = std::chrono::high_resolution_clock::now();
            m_ended = true;
        }
    }

    void Timer::pause() noexcept
    {
        std::scoped_lock<std::mutex> lock {m_mutex};

        if (!m_start.has_value() || m_paused || m_end.has_value())
        {
            return;
        }

        m_pauseStart = std::chrono::high_resolution_clock::now();
        m_paused     = true;
    }

    void Timer::resume() noexcept
    {
        std::scoped_lock<std::mutex> lock {m_mutex};

        if (!m_start.has_value() || !m_paused || m_end.has_value())
        {
            return;
        }

        auto now = std::chrono::high_resolution_clock::now();

        m_pauseDuration += now - m_pauseStart.value();
        m_paused = false;
        m_pauseStart.reset();
    }

    double Timer::getDuration() const
    {
        std::scoped_lock<std::mutex> lock {m_mutex};
        if (!m_start.has_value())
        {
            return 0.0;
        }

        auto now = std::chrono::high_resolution_clock::now();

        auto ref = m_paused ? m_pauseStart.value() : now;

        if (m_ended)
        {
            ref = m_end.value();
        }

        auto live = std::chrono::duration<double>(ref - m_start.value() - m_pauseDuration).count();

        return live;
    }

    double Timer::getTimestamp()
    {
        static auto start = std::chrono::high_resolution_clock::now();
        auto        now   = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - start).count();
    }
}  // namespace Core
