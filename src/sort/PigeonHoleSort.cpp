#include "sort/PigeonHoleSort.h"

PigeonHoleSort::PigeonHoleSort(std::vector<int>& arr, ImGuiIO* io) : Sort(arr, io) {}

void PigeonHoleSort::sort()
{
    isSorting = true;
    int n = elems.size();
    // Find minimum and maximum values in elems[]
    int min = elems[0], max = elems[0];
    for (int i = 1; i < n; i++)
    {
        if (elems[i] < min)
            min = elems[i];
        if (elems[i] > max)
            max = elems[i];
        comparisions++;
    }
 
    // Create an array of vectors. Size of array
    // max - min + 1. Each vector represents a hole that
    // is going to contain matching elements.
    std::vector<int> holes[max - min + 1];
 
    // Traverse through input array and put every
    // element in its respective hole
    for (int i = 0; i < n; i++)
        holes[elems[i]-min].push_back(elems[i]);
 
    // Traverse through all holes one by one. For
    // every hole, take its elements and put in
    // array.
    int index = 0;  // index in sorted array
    for (int i = 0; i < max - min + 1; i++)
    {
       std::vector<int>::iterator it;
       for (it = holes[i].begin(); it != holes[i].end(); ++it)
       {
            elems[index++]  = *it;
            this->sortRenderer->update(this->elems, index + 1, index + 1);
            if (wantClose || wantStop)
                return;
       }
    }
    isSorting = false;
    sorted = true;
}