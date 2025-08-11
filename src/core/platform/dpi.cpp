#include "core/platform/dpi.h"

namespace Core::Platform
{
    DPI DPI::From(SDL_Window* t_window, SDL_Renderer* t_renderer)
    {
        DPI dpi;

        int logicalWidth, logicalHeight;
        SDL_GetWindowSize(t_window, &logicalWidth, &logicalHeight);
        int outputWidth, outputHeight;
        SDL_GetRenderOutputSize(t_renderer, &outputWidth, &outputHeight);

        if (logicalWidth > 0 && logicalHeight > 0)
        {
            dpi.scaleX = static_cast<float>(outputWidth) / static_cast<float>(logicalWidth);
            dpi.scaleY = static_cast<float>(outputHeight) / static_cast<float>(logicalHeight);
        }

        return dpi;
    }
}  // namespace Core::Platform
