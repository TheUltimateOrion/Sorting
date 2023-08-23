#pragma once
#include "../sort/Sort.h"

class SortRenderer
{
public:
    static void render(Sort* sort, int a, int b, float speed);

    static void renderGUI(Sort* sort);
};