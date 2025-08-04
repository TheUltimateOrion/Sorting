#include "core/app_ctx.h"

#include "core/logging/logging.h"
#include "core/platform/display.h"

namespace Core
{
    Ctx* Ctx::createContext(float width, float height, uint16_t fps)
    {
        Ctx* ctx = new Ctx;

        SDL_DisplayID          displayID = Core::Platform::Display::getCurrentDisplayID();
        SDL_DisplayMode const* mode      = Core::Platform::Display::getDisplayMode(displayID);

        ctx->fps       = std::clamp(static_cast<int>(fps), 1, static_cast<int>(mode->refresh_rate));
        ctx->winWidth  = width / mode->pixel_density;
        ctx->winHeight = height / mode->pixel_density;

        LOGINFO("Creating SDL Window");
        ctx->window = SDL_CreateWindow("Sorting Algorithms", ctx->winWidth, ctx->winHeight,
                                       SDL_WINDOW_HIGH_PIXEL_DENSITY);

        LOGINFO("Creating SDL renderer");
        ctx->renderer = SDL_CreateRenderer(ctx->window, nullptr);

        ctx->dpi = Core::Platform::DPI::from(ctx->window, ctx->renderer);

        SDL_SetRenderScale(ctx->renderer, ctx->dpi.scaleX, ctx->dpi.scaleY);
        return ctx;
    }

    void Ctx::destroyContext(Ctx* ctx)
    {
        if (ctx->renderer)
        {
            LOGINFO("Destroying SDL renderer");
            SDL_DestroyRenderer(ctx->renderer);
        }

        LOGINFO("Destroying SDL window");
        if (ctx->window) { SDL_DestroyWindow(ctx->window); }

        delete ctx;
    }

    float Ctx::getFrameTime() const noexcept { return 1000.0f / fps; }
} // namespace Core