#include "sort/GravitySort.h"

GravitySort::GravitySort(std::vector<int>& arr) : Sort(arr) {}

void GravitySort::sort()
{
    isSorting = true;
    int max = *std::max_element(elems.begin(), elems.end());
    if (max <= 0) return;

    size_t n = elems.size();
    
    std::vector<std::vector<int>> abacus(n, std::vector<int>(max, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < elems[i]; ++j)
            abacus[i][max - j - 1] = 1;

    for (int col = 0; col < max; ++col) {
        int count = 0;
        
        for (int row = 0; row < n; ++row)
            count += abacus[row][col];

        for (int row = n - 1; row >= 0; --row)
            abacus[row][col] = (count-- > 0 ? 1 : 0);

        for (int i = 0; i < n; ++i) {
            elems[i] = std::accumulate(abacus[i].begin(), abacus[i].end(), 0);
            this->first = i;
            this->second = i;
            HIGH_RES_WAIT(1.f / Sort::speed);
            if (wantClose || wantStop) return;
        }
    }
    
    isSorting = false;
    sorted = true;
}