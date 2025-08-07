#include "core/app_ctx.h"

#include "core/logging/logging.h"
#include "core/platform/display.h"

#include <algorithm>
#include <string>

#include <cstdint>

namespace Core
{
    Ctx* Ctx::createContext(float t_width, float t_height, std::uint16_t t_fps)
    {
        Ctx*                   ctx       = new Ctx;

        SDL_DisplayID          displayID = Core::Platform::Display::getCurrentDisplayID();
        SDL_DisplayMode const* mode      = Core::Platform::Display::getDisplayMode(displayID);

        ctx->fps                         = std::clamp<std::uint16_t>(t_fps, 1, static_cast<std::uint16_t>(mode->refresh_rate));
        ctx->winWidth                    = t_width / mode->pixel_density;
        ctx->winHeight                   = t_height / mode->pixel_density;

        LOGINFO("Creating SDL Window");
        ctx->window = SDL_CreateWindow("Sorting Algorithms", ctx->winWidth, ctx->winHeight, SDL_WINDOW_HIGH_PIXEL_DENSITY);

        LOGINFO("Creating SDL renderer");
        ctx->renderer = SDL_CreateRenderer(ctx->window, nullptr);

        ctx->dpi      = Core::Platform::DPI::from(ctx->window, ctx->renderer);

        SDL_SetRenderScale(ctx->renderer, ctx->dpi.scaleX, ctx->dpi.scaleY);
        return ctx;
    }

    void Ctx::destroyContext(Ctx* t_ctx)
    {
        LOGINFO("Destroying SDL renderer");
        if (t_ctx->renderer) { SDL_DestroyRenderer(t_ctx->renderer); }

        LOGINFO("Destroying SDL window");
        if (t_ctx->window) { SDL_DestroyWindow(t_ctx->window); }

        LOGINFO("Destroying font");
        if (t_ctx->font) { TTF_CloseFont(t_ctx->font); }

        delete t_ctx;
    }

    Utils::Signal Ctx::createFont(std::string const& relativePath)
    {
        std::string basePath {SDL_GetBasePath()};
        font = TTF_OpenFont((basePath + relativePath).c_str(), 12);

        if (font == NULL)
        {
            LOGINFO(
                "Font failed to load: '" << (basePath + relativePath)
                                         << "' is not a font or does not exist"
            );
            return Utils::Signal::Error;
        }

        LOGINFO("Font loaded successfully from " << (basePath + relativePath));
        return Utils::Signal::Success;
    }

}  // namespace Core
