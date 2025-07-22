#pragma once
#include <string>

#include <SDL3/SDL.h>

#include "sort/base.h"
#include "render_params.h"

#ifndef SORTCASE
#define SORTCASE(num, typeSort)\
    case num: {\
        AppCtx::g_app->sorter = std::make_shared<typeSort>(AppCtx::g_app->data);\
        AppCtx::g_app->sorter->setLength(BaseSort::s_length);\
    } break;
#endif

#ifndef SORTCASERADIX
#define SORTCASERADIX(num, typeSort)\
    case num: {\
        AppCtx::g_app->sorter = std::make_shared<typeSort>(AppCtx::g_app->data, m_radix);\
        AppCtx::g_app->sorter->setLength(BaseSort::s_length);\
    } break;
#endif

class SortView
{
private:
    std::vector<int> m_elems;
    bool m_isColored;
    bool m_reversed;

    void drawElement(size_t k, const RenderParams& t_params) noexcept;
public:
    SortView();

    void update() noexcept;

    void renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept;
    void renderInfo() const noexcept;

    int renderGUI() noexcept;
};