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

constexpr float WIN_WIDTH = 1920.0f;
constexpr float WIN_HEIGHT = 1080.0f;

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;


#ifndef TESTING
#define LOCK_GUARD std::lock_guard<std::mutex> lock(app->m_mtx)
#else
#define LOCK_GUARD do {} while(0);
#endif

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

#ifndef TESTING

class SoundEngine;
class SortRenderer;
class Sort;

class App
{
private:
    mutable std::mutex m_mtx;

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

    std::atomic<int> current_element;

    bool isColored = false;

    std::vector<std::vector<const char *>> items;
    int current_item = 0;
    
    std::vector<const char *> categories;
    enum SortCategory current_category = SortCategory::Exchange;

    std::vector<const char *> displayTypes;
    enum DisplayType displayType = DisplayType::Bar;

    bool isRadix = false;
    int setRadix = 2;
    SDL_FRect rect;
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
    void setStyle(ImGuiStyle& style) const noexcept;
};

extern std::unique_ptr<App> app;

#endif