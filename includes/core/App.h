#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <random>
#include <memory>
#include <mutex>
#include <atomic>
#include <optional>
#include <array>

#include <cmath>

using namespace std::literals::chrono_literals;

#ifndef TESTING

#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <AL/al.h>
#include <AL/alc.h>

#include "renderer/DisplayType.h"
#include "sort/SortCategories.h"

#endif

#ifndef TESTING

class SoundEngine;
class SortRenderer;
class Sort;

class App
{
private:
    mutable std::mutex m_mutex;
    ImGuiIO* m_io;

    ImGuiIO& configureIO() noexcept;
    [[nodiscard]] int initSDL();
    [[nodiscard]] int initImGui();
    [[nodiscard]] int initAudio();

    friend class Sort;
    friend class SortRenderer;
public:
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    TTF_Font *font;

    SoundEngine* snd;

    std::unique_ptr<SortRenderer> sortRenderer;
    std::shared_ptr<Sort> sorter;

    std::atomic<size_t> currentElement;

    std::array<std::vector<const char *>, 5> sortTypes;
    size_t current_item = 0;
    
    std::array<const char *, 5> categories;
    enum SortCategory currentCategory;

    std::array<const char *, 8> displayTypes;
    enum DisplayType currentDisplayType;

    bool isRadix = false;
    int setRadix = 2;
    bool reverse = false;

    std::vector<int> data;

    std::optional<std::thread> sortThread;
    std::optional<std::thread> audioThread;

    App() noexcept;
    ~App();
    
    int init();
    void run();

    void startAudioThread();

    [[nodiscard]] int loadFont();
    void setStyle(ImGuiStyle& t_style) const noexcept;

    float getFramerate() const;
};

#endif