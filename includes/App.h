#pragma once
#include "sort/Sort.h"

class App
{
public:
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    TTF_Font *font;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    Mix_Chunk* sfx = NULL;
    int setLength = 512;
    unsigned int swaps = 0;
    unsigned int comparisions = 0;

    float setSpeed = 1.0f;
    bool isColored = false;
    int displayType = 0;
    std::vector<const char *> items;
    int current_item = 0;
    bool isRadix = false;
    int setRadix = 2;
    SDL_Rect rect;
    bool reverse = false;
    
    Sort* sorter;

    App();
    ~App();
    
    void calculateDeltaTime();
    int loadFonts();
    int loadSound();
    void setStyle(ImGuiStyle* style);
    ImGuiIO& configureIO();
};
extern App* app;