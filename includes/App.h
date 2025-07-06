#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <thread>
#include <random>
#include <memory>

#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <AL/al.h>
#include <AL/alc.h>

constexpr float WIN_WIDTH = 1920.0f;
constexpr float WIN_HEIGHT = 1080.0f;

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;

#ifndef HANDLE_ERROR
#define HANDLE_ERROR(str, ret)\
    if (snd->alGetLastError() != AL_NO_ERROR) {\
        std::cerr << str << " with code: " << snd->alErrorString(snd->alGetLastError()) << "(" << snd->alGetLastError() << ")" << std::endl;\
        return ret;\
    }
#endif

#ifndef STYLESET
#define STYLESET(param) style.Colors[ImGuiCol_##param]
#endif

class SoundEngine;
class SortRenderer;
class Sort;

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
    SortRenderer* sortRenderer;
    std::shared_ptr<Sort> sorter;

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
    SDL_FRect rect;
    bool reverse = false;

    std::vector<int> data;

    App() noexcept;
    ~App();
    
    int init();
    void run();

    void startAudioThread();

    void calculateDeltaTime() noexcept;
    ImGuiIO& configureIO() noexcept;
    [[nodiscard]] int loadFont();
    void setStyle(ImGuiStyle& style) const noexcept;
};

extern std::unique_ptr<App> app;