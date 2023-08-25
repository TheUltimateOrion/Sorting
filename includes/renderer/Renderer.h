#pragma once
#include "../sort/Sort.h"

class SortRenderer
{
public:
    static void render(Sort* sort, std::vector<int>& elems, int a, int b);

    static void renderText(std::string txt, int x, int y, SDL_Color color);

    static bool renderGUI(Sort* sort);
};