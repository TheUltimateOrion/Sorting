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

#include "renderer/sort_view.h"
#include "renderer/ui.h"
#endif

#include "core/app_ctx.h"
#include "renderer/disp_type.h"
#include "sort/category.h"
#include "sound/sound_engine.h"

#ifndef TESTING

namespace Core 
{
    class App : public std::enable_shared_from_this<App>
    {
    private:
        std::unique_ptr<Renderer::SortView> m_sortView;
        Renderer::UI m_UI;
        ImGuiIO* m_io;
        SoundEngine* m_soundEngine;
        std::optional<std::thread> m_audioThread;

        [[nodiscard]] Utils::Signal initSDL();
        [[nodiscard]] Utils::Signal initImGui();
        [[nodiscard]] Utils::Signal initAudio();
        [[nodiscard]] Utils::Signal initFont();

        ImGuiIO& configureIO() noexcept;

        friend class Sort::BaseSort;
        friend class Renderer::SortView;
    public:
        SDL_Event event;
        TTF_Font *font;

        std::shared_ptr<Sort::BaseSort> sorter;
        
        std::array<const char *, 5> categories;
        enum Sort::Category currentCategory;

        std::array<const char *, 8> displayTypes;
        enum Renderer::DisplayType currentDisplayType;

        int32_t sortRadix = 2;

        std::optional<std::thread> sortThread;

        Core::SortRegistry sortRegistry; // Global sort registry

        Core::Ctx* ctx;

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
