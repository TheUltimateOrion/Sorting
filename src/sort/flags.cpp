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

        wantClose   = false;
        wantStop    = false;
    }

    void Flags::doneSorting() noexcept
    {
        isSorting = false;
        hasSorted = true;
    }
}  // namespace Sort
