#pragma once
#include "renderer/Renderer.h"
#include "renderer/Sound.h"
#include "sort/BubbleSort.h"

#ifndef HANDLE_ERROR
#define HANDLE_ERROR(str, ret)\
    if (snd->alGetLastError() != AL_NO_ERROR) {\
        std::cerr << str << " with code: " << snd->alErrorString(snd->alGetLastError()) << "(" << snd->alGetLastError() << ")" << std::endl;\
        return ret;\
    }
#endif

#ifndef STYLESET
#define STYLESET(param) style->Colors[ImGuiCol_##param]
#endif

extern App* app;

class App
{
private:
    void _setupGUI();
public:
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    TTF_Font *font;

    SoundEngine* snd;

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
    int loadFont();
    void setStyle(ImGuiStyle* style);
};