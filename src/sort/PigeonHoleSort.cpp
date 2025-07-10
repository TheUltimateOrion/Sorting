#include "sort/PigeonHoleSort.h"

PigeonHoleSort::PigeonHoleSort(std::vector<int>& arr) : Sort(arr) {}

void PigeonHoleSort::sort()
{
    isSorting = true;
    int n = elems.size();
    if (n == 0) return;

    int min = elems[0], max = elems[0];
    for (int i = 1; i < n; i++)
    {
        if (elems[i] < min)
            min = elems[i];
            
        if (elems[i] > max)
            max = elems[i];
            
        this->first = i;
        this->second = this->first.load();
        HIGH_RES_WAIT(1.f / Sort::speed);
        if (wantClose || wantStop) return;

        comparisions += 2;
    }

    std::vector<std::vector<int>> holes(max - min + 1);

    for (int i = 0; i < n; ++i)
        holes[elems[i] - min].push_back(elems[i]);

    int index = 0;
    for (int i = 0; i < holes.size(); ++i) {
        for (int val : holes[i]) {
            elems[index] = val;
            this->first = index;
            this->second = i;
            HIGH_RES_WAIT(1.f / Sort::speed);
            if (wantClose || wantStop) return;
            index++;
        }
    }

    isSorting = false;
    sorted = true;
}