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

        double getDuration() const;

        static double getTimestamp();
    };
} // namespace Core
