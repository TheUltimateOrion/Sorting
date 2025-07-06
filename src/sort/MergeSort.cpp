#include "sort/MergeSort.h"

MergeSort::MergeSort(std::vector<int>& arr) : Sort(arr) {}

void MergeSort::merge(int const left, int const mid, int const right)
{
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;
 
    // Create temp arrays
    std::vector<int> leftArray(subArrayOne);
    std::vector<int> rightArray(subArrayTwo);
 
    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = elems[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = elems[mid + 1 + j];
 
    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;
 
    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne
           && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne]
            <= rightArray[indexOfSubArrayTwo]) {
            elems[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            app->sortRenderer->update(elems, indexOfSubArrayOne, indexOfSubArrayOne);
            if (wantClose || wantStop)
                return;
            indexOfSubArrayOne++;
        }
        else {
            elems[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            app->sortRenderer->update(elems, indexOfSubArrayTwo, indexOfSubArrayTwo);
            if (wantClose || wantStop)
                return;
            indexOfSubArrayTwo++;
        }
        comparisions++;
        indexOfMergedArray++;
    }
 
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        elems[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        app->sortRenderer->update(elems, indexOfMergedArray, indexOfSubArrayOne);
        if (wantClose || wantStop)
            return;
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
 
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        elems[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        app->sortRenderer->update(elems, indexOfMergedArray, indexOfSubArrayTwo);
        if (wantClose || wantStop)
            return;
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}
 
void MergeSort::_mergeSort(int const begin, int const end)
{
    if (begin >= end)
        return;
 
    int mid = begin + (end - begin) / 2;
    _mergeSort(begin, mid);
    if (wantClose || wantStop)
        return;
    _mergeSort(mid + 1, end);
    if (wantClose || wantStop)
        return;
    merge(begin, mid, end);
    if (wantClose || wantStop)
        return;
}

// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void MergeSort::sort()
{
    isSorting = true;
    int begin = 0;
    int end = elems.size() - 1;

    _mergeSort(begin ,end);
    if (wantClose || wantStop)
        return;
    isSorting = false;
    sorted = true;
}