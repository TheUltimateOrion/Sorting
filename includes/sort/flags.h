#pragma once

#include <atomic>

namespace Sort
{
    struct Flags
    {
        std::atomic<bool> hasSorted {true};

        std::atomic<bool> isChecking {false};
        std::atomic<bool> isRunning {false};
        std::atomic<bool> isShuffling {false};
        std::atomic<bool> isSorting {false};
        std::atomic<bool> wantClose {false};
        std::atomic<bool> wantStop {false};
        std::atomic<bool> hasRadix {false};

        void              reset() noexcept;

        void              doneSorting() noexcept;
    };
}  // namespace Sort
