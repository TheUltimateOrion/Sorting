#include "renderer/sort_view.h"

#include "core/app.h"
#include "core/logging/logging.h"
#include "renderer/context.h"
#include "renderer/disp_type.h"
#include "renderer/state.h"
#include "sort/category.h"
#include "sort/sort.h"
#include "utils/common.h"
#include "utils/renderer.h"

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <algorithm>

using namespace std::literals::chrono_literals;

namespace Renderer
{
    SortView::SortView(std::shared_ptr<Core::App> t_app, UIState& t_state) noexcept
        : m_app {t_app}, m_uiState {t_state}
    { }

    void SortView::drawElement(std::size_t t_index, RenderParams const& t_params) noexcept
    {
        std::size_t k = t_index;

        if (auto appShared = m_app.lock())
        {
            if (k >= m_elems.size()) { return; }

            Renderer::RenderContext const* const ctx = appShared->getContext();

            switch (m_uiState.sortDisplayType)
            {
                case DisplayType::Bar: {
                    SDL_FRect rect {
                        k * t_params.barSpacing, ctx->winHeight, t_params.barSpacing,
                        -(m_elems[k] * t_params.barSpacing * ctx->winHeight / ctx->winWidth)
                    };
                    SDL_RenderFillRect(ctx->renderer, &rect);
                }
                break;

                case DisplayType::Dot: {
                    SDL_RenderPoint(
                        ctx->renderer, (k + 1) * t_params.barSpacing,
                        (ctx->winHeight
                         - m_elems[k] * t_params.barSpacing * ctx->winHeight / ctx->winWidth)
                    );
                }
                break;

                case DisplayType::RainbowRectangle: {
                    m_uiState.isColored = true;
                    SDL_FRect rect {
                        k * t_params.barSpacing, 0, t_params.barSpacing, ctx->winHeight
                    };
                    SDL_RenderFillRect(ctx->renderer, &rect);
                }
                break;

                case DisplayType::Circle: {
                    m_uiState.isColored = true;
                    float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
                    SDL_GetRenderDrawColorFloat(ctx->renderer, &r, &g, &b, &a);

                    SDL_Vertex vertices[3];

                    vertices[0] = {
                        {0.5f * ctx->winWidth, 0.5f * ctx->winHeight}, // first point location
                        {r, g, b, 0xFF},
                        {0.f, 0.f}
                    };

                    vertices[1] = {
                        {0.5f * ctx->winWidth
                             + t_params.circleRadius
                                   * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k),
                         0.5f * ctx->winHeight
                             + t_params.circleRadius
                                   * sinf(
                                       t_params.degreesPerIndex * Utils::kRadiansPerDegree * k
                                   )}, // second point location
                        {r, g, b, 0xFF},
                        {0.f, 0.f}
                    };

                    vertices[2] = {
                        {0.5f * ctx->winWidth
                             + t_params.circleRadius
                                   * cosf(
                                       t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)
                                   ),
                         0.5f * ctx->winHeight
                             + t_params.circleRadius
                                   * sinf(
                                       t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)
                                   )}, // third point location
                        {r, g, b, 0xFF},
                        {0.f, 0.f}
                    };

                    if (!SDL_RenderGeometry(ctx->renderer, nullptr, vertices, 3, nullptr, 0))
                    {
                        LOGERR(SDL_GetError());
                    }
                }
                break;

                case DisplayType::CircleDot: {
                    SDL_RenderPoint(
                        ctx->renderer,
                        0.5f * ctx->winWidth
                            + t_params.circleRadius
                                  * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k),
                        0.5f * ctx->winHeight
                            + t_params.circleRadius
                                  * sinf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k)
                    );
                }
                break;

                case DisplayType::DisparityCircle: {
                    SDL_RenderLine(
                        ctx->renderer,
                        0.5f * ctx->winWidth
                            + t_params.circleRadius
                                  * cosf(
                                      t_params.degreesPerIndex * Utils::kRadiansPerDegree
                                      * m_elems[k]
                                  ),
                        0.5f * ctx->winHeight
                            + t_params.circleRadius
                                  * sinf(
                                      t_params.degreesPerIndex * Utils::kRadiansPerDegree
                                      * m_elems[k]
                                  ),
                        0.5f * ctx->winWidth
                            + t_params.circleRadius
                                  * cosf(
                                      t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)
                                  ),
                        0.5f * ctx->winHeight
                            + t_params.circleRadius
                                  * sinf(
                                      t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)
                                  )
                    );
                }
                break;

                case DisplayType::Spiral: {
                    float r = 0, g = 0, b = 0, a = 0;
                    SDL_GetRenderDrawColorFloat(ctx->renderer, &r, &g, &b, &a);

                    SDL_Vertex vertices[3];

                    vertices[0] = {
                        {0.5f * ctx->winWidth, 0.5f * ctx->winHeight}, // first point location
                        {r, g, b, 0xFF},
                        {0.f, 0.f}
                    };

                    vertices[1] = {
                        {0.5f * ctx->winWidth
                             + t_params.spiralScale * m_elems[k]
                                   * cosf(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k),
                         0.5f * ctx->winHeight
                             + t_params.spiralScale * m_elems[k]
                                   * sinf(
                                       t_params.degreesPerIndex * Utils::kRadiansPerDegree * k
                                   )}, // second point location
                        {r, g, b, 0xFF},
                        {0.f, 0.f}
                    };

                    vertices[2] = {
                        {0.5f * ctx->winWidth
                             + t_params.spiralScale * m_elems[k]
                                   * cosf(
                                       t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)
                                   ),
                         0.5f * ctx->winHeight
                             + t_params.spiralScale * m_elems[k]
                                   * sinf(
                                       t_params.degreesPerIndex * Utils::kRadiansPerDegree * (k + 1)
                                   )}, // third point location
                        {r, g, b, 0xFF},
                        {0.f, 0.f}
                    };

                    SDL_RenderGeometry(ctx->renderer, nullptr, vertices, 3, nullptr, 0);
                }
                break;

                case DisplayType::SpiralDot: {
                    SDL_RenderPoint(
                        ctx->renderer,
                        0.5f * ctx->winWidth
                            + t_params.spiralScale * m_elems[k]
                                  * cos(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k),
                        0.5f * ctx->winHeight
                            + t_params.spiralScale * m_elems[k]
                                  * sin(t_params.degreesPerIndex * Utils::kRadiansPerDegree * k)
                    );
                }
                break;
            }
        }
    }

    void SortView::update(UIState& t_uiState) noexcept
    {
        m_uiState = t_uiState;
        if (auto appShared = m_app.lock())
        {
            auto&                                sorter = appShared->getSorter();
            Renderer::RenderContext const* const ctx    = appShared->getContext();

            SDL_SetRenderDrawColor(ctx->renderer, 0x0, 0x0, 0x0, 0x0);
            SDL_RenderClear(ctx->renderer);

            m_elems = sorter->elems;

            RenderParams t_params {
                .degreesPerIndex = 360.0f / static_cast<float>(m_elems.size()),
                .barSpacing      = ctx->winWidth / m_elems.size(),
                .circleRadius    = std::min(ctx->winHeight, ctx->winWidth) * 0.75f * 0.5f,
                .spiralScale     = 0.5f * std::min(ctx->winHeight, ctx->winWidth)
                             / static_cast<float>(m_elems.size())
            };

            for (std::size_t k = 0; k < m_elems.size(); ++k)
            {
                SDL_Color sortColor = Utils::hsvToRgb(
                    m_elems[k] * 0xFF / m_elems.size(), 0xFF, 0xFF
                );

                Uint8 r = 0xFF, g = 0xFF, b = 0xFF;  // Default: white

                if (k == sorter->getFirst() || k == sorter->getSecond())
                {
                    r = 0xFF;
                    g = 0x00;
                    b = 0x00;  // Red
                }
                else if (sorter->getFlags().isChecking)
                {
                    if (k < sorter->getFirst())
                    {
                        r = 0x00;
                        g = 0xFF;
                        b = 0x00;  // Green
                    }
                    else if (m_uiState.isColored)
                    {
                        r = sortColor.r;
                        g = sortColor.g;
                        b = sortColor.b;
                    }
                }
                else if (m_uiState.isColored)
                {
                    r = sortColor.r;
                    g = sortColor.g;
                    b = sortColor.b;
                }

                SDL_SetRenderDrawColor(ctx->renderer, r, g, b, 0xFF);

                drawElement(k, t_params);
            }
        }
    }
}  // namespace Renderer
