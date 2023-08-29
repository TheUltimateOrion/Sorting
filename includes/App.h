#pragma once
#include "renderer/Renderer.h"
#include "sort/BubbleSort.h"

class App
{
private:
    void _setupGUI();
    ALenum error;
    ALuint buf;
    ALuint src = 0;
    short *samples;
public:
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    TTF_Font *font;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    int setLength = 512;
    unsigned int swaps = 0;
    unsigned int comparisions = 0;
    ImGuiIO* io;

    int current_element = 0;

    float setSpeed = 1.0f;
    bool isColored = false;

    std::vector<std::vector<const char *>> items;
    int current_item = 0;
    
    std::vector<const char *> categories;
    int current_category = 0;

    std::vector<const char *> displayTypes;
    int displayType = 0;

    bool isRadix = false;
    int setRadix = 2;
    SDL_Rect rect;
    bool reverse = false;
    
    Sort* sorter;

    App();
    ~App();
    
    int init();
    void run();

    void startAudioThread();

    void calculateDeltaTime();
    ImGuiIO& configureIO();
    int loadFonts();
    int initSound();
    void loadSound(float ms, float freq);
    void playSound();
    int closeSound();
    void setStyle(ImGuiStyle* style);
};
extern App* app;