#pragma once

#include <chrono>
#include <optional>
#include <mutex>

namespace Core
{
    class Timer
    {
    
    private:
        std::optional<std::chrono::high_resolution_clock::time_point> m_start, m_end;
        std::optional<std::chrono::high_resolution_clock::time_point> m_pauseStart;
        std::chrono::duration<double> m_pauseDuration{0.0};
        mutable std::mutex m_mutex;

        bool m_ended{false};
        bool m_paused{false};
    public:
        Timer() noexcept = default;
        ~Timer() noexcept = default;

        void start() noexcept;
        void end() noexcept;

        void pause() noexcept;
        void resume() noexcept;

        inline static void sleep(double t_ms, Timer& t_timer) noexcept
        {
            auto start = std::chrono::high_resolution_clock::now();
            t_timer.pause();
            while (true)
            {
                auto now = std::chrono::high_resolution_clock::now();
                double elapsed = std::chrono::duration<double, std::milli>(now - start).count();
                if (elapsed >= t_ms) 
                {
                    t_timer.resume();
                    break;
                }
            }
        }

        double getDuration() const;

        static double getTimestamp();
    };
} // namespace Core
