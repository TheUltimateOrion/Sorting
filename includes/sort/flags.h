#pragma once

#include <atomic>

namespace Sort
{

    enum FlagGroup
    {
        DoneSorting,
        Quit,
        SortButtonPressed,
        StopButtonPressed,
    };

    struct Flags
    {
        std::atomic<bool> hasRadix {false};
        std::atomic<bool> hasSorted {true};

        std::atomic<bool> isChecking {false};
        std::atomic<bool> isRunning {false};
        std::atomic<bool> isShuffling {false};
        std::atomic<bool> isSorting {false};

        std::atomic<bool> shouldSort {false};

        std::atomic<bool> wantClose {false};
        std::atomic<bool> wantStop {false};

        void              reset() noexcept;

        void              setFlags(FlagGroup t_group) noexcept;
    };

    void doneSorting() noexcept;
}  // namespace Sort
