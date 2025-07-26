#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <optional>
#include <array>

#ifndef TESTING

#include <imgui/imgui.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#endif

#include "renderer/disp_type.h"

#ifndef TESTING
#include "renderer/sort_view.h"
#include "renderer/ui.h"
#endif

#include "sort/category.h"
#include "sound/sound_engine.h"

#ifndef TESTING

namespace Core 
{
    class App 
    {
    private:
        mutable std::mutex m_mutex;
        std::unique_ptr<Renderer::SortView> m_sortView;
        Renderer::UI m_UI;
        ImGuiIO* m_io;
        SoundEngine* m_soundEngine;
        SDL_Window *m_window;
        std::optional<std::thread> m_audioThread;

        [[nodiscard]] Utils::Signal initSDL();
        [[nodiscard]] Utils::Signal initImGui();
        [[nodiscard]] Utils::Signal initAudio();
        [[nodiscard]] Utils::Signal initFont();

        ImGuiIO& configureIO() noexcept;

        friend class Sort::BaseSort;
        friend class Renderer::SortView;
    public:
        SDL_Renderer *renderer;
        SDL_Event event;
        TTF_Font *font;

        std::shared_ptr<Sort::BaseSort> sorter;

        std::atomic<size_t> currentElement;

        size_t currentItemIndex = 0;
        
        std::array<const char *, 5> categories;
        enum Sort::Category currentCategory;

        std::array<const char *, 8> displayTypes;
        enum Renderer::DisplayType currentDisplayType;

        std::vector<int> data;

        std::optional<std::thread> sortThread;

        App() noexcept;
        ~App();
        
        Utils::Signal init();
        void run();

        void startAudioThread();

        void setStyle(ImGuiStyle& t_style) const noexcept;

        float getFramerate() const;
    };
}
#endif
