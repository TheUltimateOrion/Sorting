#pragma once

#include <chrono>
#include <mutex>
#include <optional>

namespace Core
{
    class Timer
    {
    private:
        bool                                                          m_ended {false};
        bool                                                          m_paused {false};

        mutable std::mutex                                            m_mutex {};

        std::chrono::duration<double>                                 m_pauseDuration {0.0};
        std::optional<std::chrono::high_resolution_clock::time_point> m_start {}, m_end {};
        std::optional<std::chrono::high_resolution_clock::time_point> m_pauseStart {};

    public:
        Timer() noexcept  = default;
        ~Timer() noexcept = default;

        void               start() noexcept;
        void               end() noexcept;
        void               pause() noexcept;
        void               resume() noexcept;
        void               reset() noexcept;
        double             getDuration() const;

        inline static void Sleep(double t_ms, Timer& t_timer) noexcept
        {
            auto start = std::chrono::high_resolution_clock::now();
            t_timer.pause();
            while (true)
            {
                auto   now     = std::chrono::high_resolution_clock::now();
                double elapsed = std::chrono::duration<double, std::milli>(now - start).count();
                if (elapsed >= t_ms)
                {
                    t_timer.resume();
                    break;
                }
            }
        }

        inline static void Sleep(double t_ms) noexcept
        {
            auto start = std::chrono::high_resolution_clock::now();
            while (true)
            {
                auto   now     = std::chrono::high_resolution_clock::now();
                double elapsed = std::chrono::duration<double, std::milli>(now - start).count();
                if (elapsed >= t_ms) { break; }
            }
        }

        static double GetTimestamp();
    };
}  // namespace Core
