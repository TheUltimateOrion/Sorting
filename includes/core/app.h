#pragma once

#include <memory>
#include <optional>
#include <thread>
#include <vector>

#ifndef TESTING

    #include <imgui/imgui.h>
    #include <SDL3/SDL.h>
    #include <SDL3_ttf/SDL_ttf.h>

    #include "renderer/sort_view.h"
    #include "renderer/ui.h"
#endif

#include "core/app_ctx.h"
#include "renderer/disp_type.h"
#include "sound/sound_engine.h"

#ifndef TESTING

namespace Core
{
    class App : public std::enable_shared_from_this<App>
    {
    private:
        std::unique_ptr<Renderer::SortView> m_sortView;
        std::shared_ptr<Sort::BaseSort>     m_sorter;

        SoundEngine*                        m_soundEngine {nullptr};
        Core::Ctx*                          m_ctx {nullptr};

        Renderer::UI                        m_UI {nullptr};
        Core::SortRegistry                  m_sortRegistry {nullptr};
        SDL_Event                           m_event {};

        std::optional<std::thread>          m_audioThread {};

        void                                configureIO() noexcept;

        [[nodiscard]] Utils::Signal         initSDL();
        [[nodiscard]] Utils::Signal         initImGui();
        [[nodiscard]] Utils::Signal         initAudio();
        [[nodiscard]] Utils::Signal         initFont();

    public:
        App() noexcept = default;
        ~App();

        std::optional<std::thread>                    sortThread {};

        Utils::Signal                                 init();
        void                                          run();

        void                                          startAudioThread();
        void                                          setStyle(ImGuiStyle& t_style) const noexcept;

        inline Core::SortRegistry const&              getRegistry() const { return m_sortRegistry; }

        inline Core::Ctx const*                       getContext() const { return m_ctx; }

        inline std::shared_ptr<Sort::BaseSort> const& getSorter() const { return m_sorter; }

        inline void                                   setSorter(std::shared_ptr<Sort::BaseSort> t_sorter) { m_sorter = t_sorter; }
    };
}  // namespace Core
#endif
