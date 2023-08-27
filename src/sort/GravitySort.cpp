#include "sort/GravitySort.h"

GravitySort::GravitySort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

void GravitySort::sort()
{
    isSorting = true;
    int max = 0;
    for (auto elem : elems) {
        if (elem > max) {
            SortRenderer::render(this, this->elems, std::distance(elems.begin(), std::find(elems.begin(), elems.end(), elem)), std::distance(elems.begin(), std::find(elems.begin(), elems.end(), elem)));
            if (wantBreak)
                return;
            max = elem;
        }
    }

    std::vector<std::vector<int>> abacus(elems.size(), std::vector<int>(max, 0));
    for (int i = 0; i < elems.size(); i++)
    {
        for (int j = 0; j < elems[i]; j++)
        {
            abacus[i][abacus[0].capacity() - j - 1] = 1;
        }
    }

    for (int i = 0; i < abacus[0].capacity(); i++)
    {
        for (int j = 0; j < abacus.capacity(); j++)
        {
            if (abacus[j][i] == 1)
            {
                int drops = j;
                while (drops + 1 < abacus.capacity() && abacus[drops][i] == 1)
                    drops++;
                if (abacus[drops][i] == 0)
                {
                    abacus[j][i] = 0;
                    abacus[drops][i] = 1;
                }
            }
        }

        int count = 0;
        for (int x = 0; x < abacus.capacity(); x++)
        {
            count = 0;
            for(int y = 0; y < abacus[0].capacity(); y++)
                    count+=abacus[x][y];
            elems[x] = count;
            SortRenderer::render(this, this->elems, x, x);
            if (wantBreak)
                return;
        }
    }
    isSorting = false;
    sorted = true;
}