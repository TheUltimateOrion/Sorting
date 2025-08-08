#include "core/platform/display.h"

#include <imgui.h>

namespace Core::Platform::Display
{
    SDL_DisplayID          getCurrentDisplayID() { return SDL_GetPrimaryDisplay(); }

    SDL_DisplayMode const* getDisplayMode(SDL_DisplayID t_id)
    {
        return SDL_GetCurrentDisplayMode(t_id);
    }

    float getFramerate() { return ImGui::GetIO().Framerate; }
}  // namespace Core::Platform::Display
