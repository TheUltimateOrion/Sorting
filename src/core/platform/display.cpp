#include "core/platform/display.h"

namespace Core::Platform::Display
{
    SDL_DisplayID getCurrentDisplayID()
    {
        return SDL_GetPrimaryDisplay();
    }

    SDL_DisplayMode const *getDisplayMode(SDL_DisplayID id)
    {
        return SDL_GetCurrentDisplayMode(id);
    }
}