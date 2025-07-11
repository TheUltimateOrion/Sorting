#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <thread>
#include <random>
#include <memory>
#include <mutex>
#include <atomic>
#include <optional>
#include <array>

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
#include "core/logging/Logging.h"
#include "core/AppCtx.h"

#endif

#ifndef TESTING
#define LOCK_GUARD std::lock_guard<std::mutex> lock(AppCtx::g_app->m_mutex)
#else
#define LOCK_GUARD do {} while(0);
#endif

#ifndef STYLESET
#define STYLESET(param) t_style.Colors[ImGuiCol_##param]
#endif

#ifndef TESTING

class SoundEngine;
class SortRenderer;
class Sort;


class App
{
private:
    mutable std::mutex m_mutex;

    friend class Sort;
    friend class SortRenderer;
private:
    void _setupGUI();
public:
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    TTF_Font *font;

    SoundEngine* snd;
    std::unique_ptr<SortRenderer> sortRenderer;
    std::shared_ptr<Sort> sorter;

    ImGuiIO* io;

    std::atomic<size_t> currentElement;

    bool isColored = false;

    std::vector<std::vector<const char *>> items;
    size_t current_item = 0;
    
    std::array<const char *, 5> categories;
    enum SortCategory currentCategory = SortCategory::Exchange;

    std::array<const char *, 8> displayTypes;
    enum DisplayType currentDisplayType = DisplayType::Bar;

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

    ImGuiIO& configureIO() noexcept;
    [[nodiscard]] int loadFont();
    void setStyle(ImGuiStyle& t_style) const noexcept;
};

#endif