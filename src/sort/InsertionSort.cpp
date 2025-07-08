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
            this->first = j;
            this->second = j - 1;
            HIGH_RES_WAIT(1.f / Sort::speed);
            if (wantClose || wantStop) return;
            --j;
        }

        elems[j] = temp;
        this->first = j;
        this->second = i;
        HIGH_RES_WAIT(1.f / Sort::speed);
        if (wantClose || wantStop) return;
    }
    isSorting = false;
    sorted = true;
}