#pragma once
#include "sort/Sort.h"
#include "renderer/RenderParams.h"

#ifndef SORTCASE
#define SORTCASE(num, typeSort)\
    case num: {\
        AppCtx::g_app->sorter = std::make_shared<typeSort>(AppCtx::g_app->data);\
        AppCtx::g_app->sorter->setLength(Sort::length);\
    } break;
#endif

#ifndef SORTCASERADIX
#define SORTCASERADIX(num, typeSort)\
    case num: {\
        AppCtx::g_app->sorter = std::make_shared<typeSort>(AppCtx::g_app->data, AppCtx::g_app->setRadix);\
        AppCtx::g_app->sorter->setLength(Sort::length);\
    } break;
#endif

class SortRenderer
{
private:
    std::vector<int> m_elems;
    bool m_isColored;

    void drawElement(size_t k, const RenderParams& t_params) noexcept;
public:
    SortRenderer();

    void update() noexcept;

    void renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept;
    void renderInfo() const noexcept;

    int renderGUI() noexcept;
};