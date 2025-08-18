#include "renderer/context.h"

#include "core/logging/logging.h"
#include "core/platform/display.h"

#include <algorithm>
#include <filesystem>
#include <string>

#include <cstdint>

namespace Renderer
{
    RenderContext* RenderContext::CreateContext(float t_width, float t_height, std::uint16_t t_fps)
    {
        RenderContext*         ctx       = new RenderContext;

        SDL_DisplayID          displayID = Core::Platform::Display::getCurrentDisplayID();
        SDL_DisplayMode const* mode      = Core::Platform::Display::getDisplayMode(displayID);

        ctx->fps                         = std::clamp<std::uint16_t>(t_fps, 1, static_cast<std::uint16_t>(mode->refresh_rate));
        ctx->winWidth                    = t_width / mode->pixel_density;
        ctx->winHeight                   = t_height / mode->pixel_density;

        LOG_INFO("Creating SDL Window");

        SDL_WindowFlags windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;

#if defined(__ANDROID__)
        ctx->winWidth  = mode->w;
        ctx->winHeight = mode->h;
        windowFlags |= SDL_WINDOW_MAXIMIZED | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN;
#endif

        ctx->window = SDL_CreateWindow(std::format("OrionSort v{}", APP_VERSION).c_str(), ctx->winWidth, ctx->winHeight, windowFlags);

        LOG_INFO("Creating SDL renderer");
        ctx->renderer = SDL_CreateRenderer(ctx->window, nullptr);

        // Make sure we draw over the full backbuffer
        // SDL_SetRenderViewport(ctx->renderer, nullptr);
        // SDL_SetRenderClipRect(ctx->renderer, nullptr);

        ctx->dpi      = Core::Platform::DPI::From(ctx->window, ctx->renderer);

        SDL_SetRenderScale(ctx->renderer, ctx->dpi.scaleX, ctx->dpi.scaleY);
        return ctx;
    }

    void RenderContext::DestroyContext(RenderContext* t_ctx)
    {
        LOG_INFO("Destroying SDL renderer");
        if (t_ctx->renderer) { SDL_DestroyRenderer(t_ctx->renderer); }

        LOG_INFO("Destroying SDL window");
        if (t_ctx->window) { SDL_DestroyWindow(t_ctx->window); }

        LOG_INFO("Destroying font");
        if (t_ctx->font) { TTF_CloseFont(t_ctx->font); }

        delete t_ctx;
    }

    Utils::Signal RenderContext::createFont(std::string const& t_relativePath)
    {
#if defined(__ANDROID__)
        std::filesystem::path basePath = "";  // on Android we do not want to use basepath. Instead, assets are available at the root directory.
#else
        std::filesystem::path const basePath {SDL_GetBasePath()};
#endif
        const std::filesystem::path fontPath = basePath / t_relativePath;
        font                                 = TTF_OpenFont(fontPath.string().c_str(), 12);

        if (!font)
        {
            LOG_INFO(
                "Font failed to load: '" << fontPath.string()
                                         << "' is not a font or does not exist"
            );
            return Utils::Signal::Error;
        }

        LOG_INFO("Font loaded successfully from " << fontPath.string());
        return Utils::Signal::Success;
    }

}  // namespace Renderer
