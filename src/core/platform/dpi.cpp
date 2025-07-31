#include "core/platform/dpi.h"

namespace Core::Platform
{
    DPI::DPI() noexcept : scaleX(1.0f), scaleY(1.0f) {}

    DPI DPI::from(SDL_Window *window, SDL_Renderer *renderer)
    {
        DPI dpi;

        int logicalWidth, logicalHeight;
        SDL_GetWindowSize(window, &logicalWidth, &logicalHeight);
        int outputWidth, outputHeight;
        SDL_GetRenderOutputSize(renderer, &outputWidth, &outputHeight);

        if (logicalWidth > 0 && logicalHeight > 0) 
        {
            dpi.scaleX = static_cast<float>(outputWidth) / static_cast<float>(logicalWidth);
            dpi.scaleY = static_cast<float>(outputHeight) / static_cast<float>(logicalHeight);
        }

        return dpi;
    }
}