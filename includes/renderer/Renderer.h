#pragma once
#include "../sort/Sort.h"

class SortRenderer
{
public:
    static void render(Sort* sort, std::vector<int>& elems, int a, int b);

    static void renderText(std::string txt, int x, int y, SDL_Color color);
    static void renderInfo(Sort*& sort);

    static SDL_Color HSVToRGB(unsigned char hue, unsigned char sat, unsigned char value);

    static bool renderGUI(Sort* sort);
};