#include "sort/SelectionSort.h"


SelectionSort::SelectionSort(std::vector<int>& arr) : Sort(arr) {}

void SelectionSort::sort()
{
    isSorting = true;
    int size = elems.size();

    for (int i = 0; i < size - 1; i ++)
    {
        int min = i;
        for (int j = i + 1; j < size; j++)
        {
            this->first = j;
            this->second = min;
            comparisions++;

            if (elems[j] < elems[min])
                min = j;

            HIGH_RES_WAIT(1.f / Sort::speed);
            if (wantClose || wantStop) return;

        }
        if (min != i)
            swap(elems, min, i);

        if (wantClose || wantStop) return;
    }

    isSorting = false;
    sorted = true;
}