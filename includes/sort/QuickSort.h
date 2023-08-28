#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class QuickSort : virtual public Sort
{
private:
    int partition(std::vector<int>& arr, int low, int high);
    void quickSort(std::vector<int>& arr, int low, int high);
public:
    QuickSort(std::vector<int>& arr, ImGuiIO* io);

    void sort() override;
};