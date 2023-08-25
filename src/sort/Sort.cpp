#include "renderer/Renderer.h"

Sort::Sort(std::vector<int>& arr, ImGuiIO& io) : elems(arr), isSorting(false), isShuffling(false), sorted(true), wantBreak(false), speed(speed), io(io), start_time(0) {}

void Sort::shuffle()
{
    isShuffling = true;
    std::vector<int> temp(elems.capacity());
    for (int i = 0; i < elems.capacity(); i++)
        temp[i] = elems[i];
    std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(0));
    for (int i = 0; i < temp.capacity(); i++)
    {
        SortRenderer::render(this, this->elems, i, i);
        if (wantBreak)
            return;
        elems[i] = temp[i];
    }
    sorted = false;
    isShuffling = false;
    SDL_Delay(500);
    this->start_time = (float)clock() / 1000.0f;
    ::last_time = this->start_time;
}

void Sort::setSpeed(float speed)
{
    this->speed = speed;
}

void Sort::swap(std::vector<int>& array, int a, int b)
{
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}
