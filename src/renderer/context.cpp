#include "renderer/context.h"

#include "core/logging/logging.h"
#include "core/platform/display.h"
#include "utils/renderer.h"

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

        ctx->fps                         = std::clamp<std::uint16_t>(
            t_fps, 1, static_cast<std::uint16_t>(mode->refresh_rate)
        );
        ctx->winWidth  = t_width / mode->pixel_density;
        ctx->winHeight = t_height / mode->pixel_density;

        LOG_INFO("Creating SDL Window");

        SDL_WindowFlags windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;

#if defined(__ANDROID__)
        ctx->winWidth  = static_cast<float>(mode->w) / Utils::kAndroidRenderScale;
        ctx->winHeight = static_cast<float>(mode->h) / Utils::kAndroidRenderScale;
        windowFlags |= SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE;
#endif

        ctx->window = SDL_CreateWindow(
            std::format("OrionSort v{}", APP_VERSION).c_str(), static_cast<int>(ctx->winWidth),
            static_cast<int>(ctx->winHeight), windowFlags
        );

        LOG_INFO("Creating SDL renderer");
        ctx->renderer = SDL_CreateRenderer(ctx->window, nullptr);

        SDL_SetRenderScale(ctx->renderer, mode->pixel_density, mode->pixel_density);
#if defined(__ANDROID__)
        SDL_SetRenderScale(ctx->renderer, Utils::kAndroidRenderScale, Utils::kAndroidRenderScale);
    #undef ANDROID_SCALE
#endif
        return ctx;
    }

    void RenderContext::DestroyContext(RenderContext* t_ctx)
    {
        LOG_INFO("Destroying font");
        if (t_ctx->font) { TTF_CloseFont(t_ctx->font); }

        LOG_INFO("Destroying SDL renderer");
        if (t_ctx->renderer) { SDL_DestroyRenderer(t_ctx->renderer); }

        LOG_INFO("Destroying SDL window");
        if (t_ctx->window) { SDL_DestroyWindow(t_ctx->window); }

        delete t_ctx;
    }

    Utils::Signal RenderContext::createFont(std::string const& t_relativePath)
    {
#if defined(__ANDROID__)
        std::filesystem::path basePath = "";  // on Android we do not want to use basepath. Instead,
                                              // assets are available at the root directory.
#else
        std::filesystem::path const basePath {SDL_GetBasePath()};
#endif
        const std::filesystem::path fontPath = basePath / t_relativePath;
        font                                 = TTF_OpenFont(fontPath.string().c_str(), 12);

        if (!font)
        {
            LOG_INFO(
                "Font failed to load: '" << fontPath.string()
                                         << "' is not a font or does not exist." << SDL_GetError();
            );
            return Utils::Signal::Error;
        }

        LOG_INFO("Font loaded successfully from " << fontPath.string());
        return Utils::Signal::Success;
    }

    ANDROID_API void RenderContext::refreshOrientation()
    {
        SDL_DisplayID          displayID = Core::Platform::Display::getCurrentDisplayID();
        SDL_DisplayMode const* mode      = Core::Platform::Display::getDisplayMode(displayID);

        winWidth                         = static_cast<float>(mode->w) / Utils::kAndroidRenderScale;
        winHeight                        = static_cast<float>(mode->h) / Utils::kAndroidRenderScale;

        SDL_SetWindowSize(window, static_cast<int>(winWidth), static_cast<int>(winHeight));
    }

}  // namespace Renderer
