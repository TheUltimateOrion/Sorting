#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class QuickSort : public Sort
{
private:
    int partition(std::vector<int>& arr, int low, int high, float speed);
    void quickSort(std::vector<int>& arr, int low, int high, float speed);
public:
    QuickSort(std::vector<int>& arr, ImGuiIO& io);

    void sort(float speed = 1) override;
};