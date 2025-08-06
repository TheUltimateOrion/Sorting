#include "sort/flags.h"

namespace Sort
{

    void Flags::reset() noexcept
    {
        hasSorted   = true;

        isChecking  = false;
        isRunning   = false;
        isShuffling = false;
        isSorting   = false;

        shouldSort  = false;

        wantClose   = false;
        wantStop    = false;
    }

    void Flags::setFlags(FlagGroup group) noexcept
    {
        switch (group)
        {
            case FlagGroup::DoneSorting:
                isSorting = false;
                hasSorted = true;
                break;
            case FlagGroup::SortButtonPressed:
                shouldSort = true;
                isRunning  = true;
                break;
            case FlagGroup::StopButtonPressed:
                isRunning  = false;
                shouldSort = false;
                wantStop   = true;
                break;
            case FlagGroup::Quit:
                isRunning = false;
                wantStop  = true;
                wantClose = true;
                break;
        }
    }
}  // namespace Sort
