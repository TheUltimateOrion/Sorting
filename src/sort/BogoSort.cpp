#include "sort/BogoSort.h"

BogoSort::BogoSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

bool BogoSort::isSorted(std::vector<int>& elems, int n)
{
    while (--n > 0) {
        if (elems[n] < elems[n - 1])
            return false;
    }
    return true;
}

void BogoSort::bogoShuffle(std::vector<int>& elems, int n)
{
    for (int i = 0; i < n; i++) {
        int randInt = rand() % n;
        SortRenderer::render(this, this->elems, i, randInt);
        if (wantClose || wantStop)
            return;
        swap(elems, i, randInt);
    }
}

void BogoSort::sort()
{
    isSorting = true;

    while(!isSorted(elems, elems.size())) {
        bogoShuffle(elems, elems.size());
        if (wantClose || wantStop)
            return;
    }
    isSorting = false;
    sorted = true;
}