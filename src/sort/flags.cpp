#include "sort/flags.h"

namespace Sort
{

    void Flags::reset() noexcept
    {
        hasAborted  = false;
        hasQuit     = false;

        isChecking  = false;
        isRunning   = false;
        isShuffling = false;
        isSorting   = false;
        isSorted    = false;

        shouldSort  = false;
    }

    void Flags::setFlags(FlagGroup group) noexcept
    {
        switch (group)
        {
            case FlagGroup::CreatingThread:
                isChecking  = false;
                isRunning   = true;
                isShuffling = false;
                isSorting   = false;
                isSorted    = false;

                shouldSort  = false;
                break;
            case FlagGroup::StartShuffling:
                isShuffling = true;
                break;
            case FlagGroup::DoneShuffling:
                isShuffling = false;
                break;
            case FlagGroup::StartSorting:
                isSorting = true;
                break;
            case FlagGroup::DoneSorting:
                isSorting = false;
                break;
            case FlagGroup::StartChecking:
                isChecking = true;
                break;
            case FlagGroup::DoneChecking:
                isChecking = false;
                isRunning  = false;
                isSorted   = true;
                break;
            case FlagGroup::SortButtonPressed:
                hasAborted = false;
                shouldSort = true;
                break;
            case FlagGroup::Quit:
                hasQuit = true;
                [[fallthrough]];
            case FlagGroup::StopButtonPressed:
                hasAborted  = true;

                isChecking  = false;
                isRunning   = false;
                isShuffling = false;
                isSorted    = false;
                isSorting   = false;

                shouldSort  = false;
                break;
        }
    }
}  // namespace Sort
