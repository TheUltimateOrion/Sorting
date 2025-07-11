#pragma once
#include "../sort/Sort.h"

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
    SDL_Color HSVToRGB(unsigned char t_hue, unsigned char t_saturation, unsigned char t_value) const noexcept;
    std::vector<int> m_elems;
public:
    void update() noexcept;

    void renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept;
    void renderInfo() const noexcept;


    int renderGUI() const noexcept;
};