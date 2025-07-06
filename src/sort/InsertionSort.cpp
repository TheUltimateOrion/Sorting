#include "sort/InsertionSort.h"

InsertionSort::InsertionSort(std::vector<int>& arr) : Sort(arr) {}

void InsertionSort::sort()
{
    isSorting = true;
    int size = elems.size();
    for (int i = 1; i < size; i++)
    {
        int j = i;
        int temp = elems[i];
        while (j > 0 && elems[j - 1] > temp)
        {
            elems[j] = elems[j - 1];
            app->sortRenderer->update(elems, j, j - 1);
            if (wantClose || wantStop)
                return;
            --j;
        }
        elems[j] = temp;
    }
    isSorting = false;
    sorted = true;
}