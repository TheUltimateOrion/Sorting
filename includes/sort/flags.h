#pragma once

#include <atomic>

namespace Sort
{

    enum FlagGroup
    {
        CreatingThread,

        StartShuffling,
        DoneShuffling,

        StartSorting,
        DoneSorting,

        StartChecking,
        DoneChecking,

        Quit,
        SortButtonPressed,
        StopButtonPressed,
    };

    struct Flags
    {
        std::atomic<bool> hasAborted {false};
        std::atomic<bool> hasQuit {false};

        std::atomic<bool> isChecking {false};
        std::atomic<bool> isRunning {false};
        std::atomic<bool> isShuffling {false};
        std::atomic<bool> isSorting {false};
        std::atomic<bool> isSorted {true};

        std::atomic<bool> shouldSort {false};

        void              reset() noexcept;

        void              setFlags(FlagGroup t_group) noexcept;
    };

    void doneSorting() noexcept;
}  // namespace Sort
