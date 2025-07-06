#pragma once
#include "../sort/Sort.h"

#ifndef SORTCASE
#define SORTCASE(num, typeSort)\
    case num: {\
        app->sorter = std::make_shared<typeSort>(app->data);\
        goto _jmp;\
    } break;
#endif

#ifndef SORTCASERADIX
#define SORTCASERADIX(num, typeSort)\
    case num: {\
        app->sorter = std::make_shared<typeSort>(app->data, app->setRadix);\
        goto _jmp;\
    } break;
#endif

class SortRenderer
{
public:
    void update(std::vector<int>& elems, int a, int b) const noexcept;

    void renderText(const std::string& txt, float x, float y, SDL_Color color) const noexcept;
    void renderInfo() const noexcept;

    SDL_Color HSVToRGB(unsigned char hue, unsigned char sat, unsigned char value) const noexcept;

    int renderGUI() const noexcept;
};