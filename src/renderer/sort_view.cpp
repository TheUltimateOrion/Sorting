
#include "renderer/sort_view.h"

#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>

#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"
#include "renderer/disp_type.h"
#include "renderer/state.h"
#include "sort/category.h"
#include "sort/sort.h"
#include "utils/common.h"
#include "utils/renderer.h"

using namespace std::literals::chrono_literals;

namespace Renderer 
{
    void SortView::drawElement(size_t k, const RenderParams& t_params) noexcept 
    {
        if (k >= m_elems.size()) 
        {
            return;
        }

        switch (AppCtx::g_app->currentDisplayType) 
        {
            case DisplayType::Bar: 
            {
                SDL_FRect rect = (SDL_FRect) {k * t_params.barSpacing, AppCtx::kWinHeight, t_params.barSpacing, -(m_elems[k] * t_params.barSpacing * AppCtx::kWinHeight / AppCtx::kWinWidth)};
                SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);
            } break;

            case DisplayType::Dot: 
            {
                SDL_RenderPoint(AppCtx::g_app->renderer, (k + 1) * t_params.barSpacing, (AppCtx::kWinHeight - m_elems[k] * t_params.barSpacing * AppCtx::kWinHeight / AppCtx::kWinWidth)); 
            } break;

            case DisplayType::RainbowRectangle: 
            {
                State::isColored = true;
                SDL_FRect rect = (SDL_FRect){k * t_params.barSpacing, 0, t_params.barSpacing, AppCtx::kWinHeight};
                SDL_RenderFillRect(AppCtx::g_app->renderer, &rect);
            } break;

            case DisplayType::Circle: 
            {
                State::isColored = true;
                float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
                SDL_GetRenderDrawColorFloat(AppCtx::g_app->renderer, &r, &g, &b, &a);
                    
                SDL_Vertex vertices[3];

                vertices[0] = {
                    {
                        0.5f * AppCtx::kWinWidth, 
                        0.5f * AppCtx::kWinHeight
                    }, // first point location
                    { r, g, b, 0xFF },
                    { 0.f, 0.f }
                };

                vertices[1] = {
                    {
                        0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k), 
                        0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k)
                    }, // second point location
                    { r, g, b, 0xFF },
                    { 0.f, 0.f }
                };

                vertices[2] = {
                    {
                        0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)), 
                        0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1))
                    }, // third point location
                    { r, g, b, 0xFF },
                    { 0.f, 0.f }
                };

                if (!SDL_RenderGeometry(AppCtx::g_app->renderer, nullptr, vertices, 3, nullptr, 0))
                {
                    LOGERR(SDL_GetError());
                }
            } break;

            case DisplayType::CircleDot: 
            {
                SDL_RenderPoint(
                    AppCtx::g_app->renderer, 
                    0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k), 
                    0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k)
                );
            } break;

            case DisplayType::DisparityCircle: 
            {
                SDL_RenderLine(
                    AppCtx::g_app->renderer, 
                    0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * m_elems[k]), 
                    0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * m_elems[k]), 
                    0.5f * AppCtx::kWinWidth + t_params.circleRadius * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)), 
                    0.5f * AppCtx::kWinHeight + t_params.circleRadius * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1))
                );
            } break;

            case DisplayType::Spiral: 
            {
                float r = 0, g = 0, b = 0, a = 0;
                SDL_GetRenderDrawColorFloat(AppCtx::g_app->renderer, &r, &g, &b, &a);
                
                SDL_Vertex vertices[3];
                
                vertices[0] = {
                    {
                        0.5f * AppCtx::kWinWidth, 
                        0.5f * AppCtx::kWinHeight
                    }, // first point location
                    { r, g, b, 0xFF },
                    { 0.f, 0.f }
                };

                vertices[1] = {
                    {
                        0.5f * AppCtx::kWinWidth + t_params.spiralScale * m_elems[k] * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k), 
                        0.5f * AppCtx::kWinHeight + t_params.spiralScale * m_elems[k] * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k)
                    }, // second point location
                    { r, g, b, 0xFF },
                    { 0.f, 0.f }
                };

                vertices[2] = {
                    {
                        0.5f * AppCtx::kWinWidth + t_params.spiralScale * m_elems[k] * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)), 
                        0.5f * AppCtx::kWinHeight + t_params.spiralScale * m_elems[k] * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1))
                    }, // third point location
                    { r, g, b, 0xFF },
                    { 0.f, 0.f }
                };

                SDL_RenderGeometry(AppCtx::g_app->renderer, nullptr, vertices, 3, nullptr, 0);
            } break;

            case DisplayType::SpiralDot: 
            {
                SDL_RenderPoint(
                    AppCtx::g_app->renderer, 
                    0.5f * AppCtx::kWinWidth + t_params.spiralScale * m_elems[k] * cos(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k), 
                    0.5f * AppCtx::kWinHeight + t_params.spiralScale * m_elems[k] * sin(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k)
                ); 
            } break;
        }
    }

    void SortView::update() noexcept
    {
        auto& sorter = AppCtx::g_app->sorter;

        SDL_SetRenderDrawColor(AppCtx::g_app->renderer, 0x0, 0x0, 0x0, 0x0);
        SDL_RenderClear(AppCtx::g_app->renderer);

        RenderParams t_params 
        {
            .degreesPerIndex = 360.0f / static_cast<float>(m_elems.size()),
            .barSpacing = AppCtx::kWinWidth / m_elems.size(),
            .circleRadius = std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) * 0.75 * 0.5,
            .spiralScale = 0.5f * std::min(AppCtx::kWinHeight, AppCtx::kWinWidth) / static_cast<float>(m_elems.size())
        };

        {
            LOCK_GUARD;
            m_elems = AppCtx::g_app->data;
        }

        AppCtx::g_app->currentElement = sorter->getFirst();

        for (size_t k = 0; k < m_elems.size(); ++k)
        {
            SDL_Color sortColor = Utils::hsvToRgb(m_elems[k] * 0xFF / m_elems.size(), 0xFF, 0xFF);

            Uint8 r = 0xFF, g = 0xFF, b = 0xFF;  // Default: white

            if (k == sorter->getFirst() || k == sorter->getSecond()) 
            {
                r = 0xFF; g = 0x00; b = 0x00;  // Red
            } 
            else if (sorter->isChecking) 
            {
                if (k < sorter->getFirst()) 
                {
                    r = 0x00; g = 0xFF; b = 0x00;  // Green
                } 
                else if (State::isColored) 
                {
                    r = sortColor.r; g = sortColor.g; b = sortColor.b;
                }
            } 
            else if (State::isColored) 
            {
                r = sortColor.r; g = sortColor.g; b = sortColor.b;
            }

            SDL_SetRenderDrawColor(AppCtx::g_app->renderer, r, g, b, 0xFF);

            drawElement(k, t_params);
        }
    }
} // namespace Renderer
