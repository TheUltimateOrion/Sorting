#include "sort/PigeonHoleSort.h"

PigeonHoleSort::PigeonHoleSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

void PigeonHoleSort::sort()
{
    isSorting = true;
    int n = elems.capacity();
    // Find minimum and maximum values in elems[]
    int min = elems[0], max = elems[0];
    for (int i = 1; i < n; i++)
    {
        if (elems[i] < min)
            min = elems[i];
        if (elems[i] > max)
            max = elems[i];
    }
    int range = max - min + 1; // Find range
 
    // Create an array of vectors. Size of array
    // range. Each vector represents a hole that
    // is going to contain matching elements.
    std::vector<int> holes[range];
 
    // Traverse through input array and put every
    // element in its respective hole
    for (int i = 0; i < n; i++)
        holes[elems[i]-min].push_back(elems[i]);
 
    // Traverse through all holes one by one. For
    // every hole, take its elements and put in
    // array.
    int index = 0;  // index in sorted array
    for (int i = 0; i < range; i++)
    {
       std::vector<int>::iterator it;
       for (it = holes[i].begin(); it != holes[i].end(); ++it)
       {
            SortRenderer::render(this, index + 1, index + 1);
            if (wantBreak)
                return;
            elems[index++]  = *it;
       }
    }
    isSorting = false;
    sorted = true;
}