#pragma once
#include "../sort/Sort.h"

class SortRenderer
{
public:
    void update(std::vector<int>& elems, int a, int b);

    void renderText(std::string txt, int x, int y, SDL_Color color);
    void renderInfo();

    SDL_Color HSVToRGB(unsigned char hue, unsigned char sat, unsigned char value);

    int renderGUI();
};