#pragma once

#include <array>
#include <memory>
#include <optional>
#include <thread>
#include <vector>

#ifndef TESTING

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <imgui/imgui.h>

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
        Renderer::UI                        m_UI;
        ImGuiIO*                            m_io;
        SoundEngine*                        m_soundEngine;
        std::optional<std::thread>          m_audioThread;

        [[nodiscard]] Utils::Signal initSDL();
        [[nodiscard]] Utils::Signal initImGui();
        [[nodiscard]] Utils::Signal initAudio();
        [[nodiscard]] Utils::Signal initFont();

        ImGuiIO& configureIO() noexcept;

        friend class Sort::BaseSort;
        friend class Renderer::SortView;

    public:
        std::shared_ptr<Sort::BaseSort> sorter;
        SDL_Event                       event;
        TTF_Font*                       font;
        enum Sort::Category             currentCategory;
        enum Renderer::DisplayType      currentDisplayType;
        std::array<const char*, 5>      categories;
        std::array<const char*, 8>      displayTypes;
        int                             sortRadix = 2;
        std::optional<std::thread>      sortThread;
        Core::SortRegistry              sortRegistry;
        Core::Ctx*                      ctx;

        App() noexcept;
        ~App();

        Utils::Signal init();
        void          run();
        void          startAudioThread();
        void          setStyle(ImGuiStyle& t_style) const noexcept;
    };
} // namespace Core
#endif
