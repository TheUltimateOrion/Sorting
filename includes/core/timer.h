#pragma once
#include <chrono>
#include <optional>

namespace Core
{
    class Timer
    {
    
    private:
        std::optional<std::chrono::high_resolution_clock::time_point> m_start, m_end;
        std::chrono::duration<double> m_duration;

        bool m_ended = false;
    public:
        Timer() noexcept = default;
        ~Timer() noexcept = default;

        void start() noexcept;
        void end() noexcept;

        double getCurrentDuration() const;
    };
} // namespace Core
