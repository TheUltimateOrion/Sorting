#include "renderer/Renderer.h"

Sort::Sort(std::vector<int>& arr) : elems(arr), isSorting(false), isShuffling(false), sorted(true), wantClose(false), wantStop(false), speed(speed), start_time(0) {}

void Sort::reverse()
{
    LOGINFO("Reversing");
    isShuffling = true;
    this->wantStop = false;
    this->swaps = 0;
    std::vector<int> temp(elems.size());
    for (int i = 0; i < elems.size(); i++)
        temp[i] = elems[i];
    std::reverse(temp.begin(), temp.end());
    for (int i = 0; i < temp.capacity(); i++)
    {
        this->sortRenderer->update(this->elems, i, i);
        if (wantClose || wantStop)
            return;
        elems[i] = temp[i];
    }
    sorted = false;
    isShuffling = false;
    SDL_Delay(500);
    this->start_time = (float)clock() / 1000.0f;
    ::last_time = this->start_time;
}

void Sort::shuffle()
{
    LOGINFO("Shuffling");
    isShuffling = true;
    this->wantStop = false;
    this->swaps = 0;
    std::vector<int> temp(elems.size());
    for (int i = 0; i < elems.size(); i++)
        temp[i] = elems[i];
    std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(0));
    for (int i = 0; i < temp.capacity(); i++)
    {
        this->sortRenderer->update(this->elems, i, i);
        if (wantClose || wantStop)
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
    swaps++;
}

void Sort::setLength(unsigned int len)
{
    LOGINFO("Resizing to " << len);
    elems.resize(len);
    for (int index = 0; index < elems.size(); index++)
        elems[index] = index + 1;
}