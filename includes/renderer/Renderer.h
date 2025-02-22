#pragma once
#include "../sort/Sort.h"

#ifndef SORTCASE
#define SORTCASE(num, typeSort)\
    case num: {\
        app->sorter = new typeSort(app->sorter->elems);\
        goto _jmp;\
    } break;
#endif

#ifndef SORTCASERADIX
#define SORTCASERADIX(num, typeSort)\
    case num: {\
        app->sorter = new typeSort(app->sorter->elems, app->setRadix);\
        goto _jmp;\
    } break;
#endif

class SortRenderer
{
public:
    void update(std::vector<int>& elems, int a, int b);

    void renderText(std::string txt, float x, float y, SDL_Color color);
    void renderInfo();

    SDL_Color HSVToRGB(unsigned char hue, unsigned char sat, unsigned char value);

    int renderGUI();
};