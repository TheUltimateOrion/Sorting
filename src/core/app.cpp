#include "core/app.h"

#include "imgui/backend/imgui_impl_sdl3.h"
#include "imgui/backend/imgui_impl_sdlrenderer3.h"

#include "core/logging/logging.h"
#include "core/platform/dpi.h"
#include "sort/exchange/bubble.h"
#include "utils/common.h"

#include <algorithm>
#include <chrono>
#include <ratio>

#include <cstdio>

using namespace std::literals::chrono_literals;

namespace Core
{
    App::~App()
    {
        LOGINFO("Joining and Destroying Sorting Thread");
        Utils::terminateThread(sortThread);

        if (m_audioThread.has_value())
        {
            if (m_audioThread->joinable())
            {
                LOGINFO("Joining Audio Thread");
                m_audioThread->join();
            }
            m_audioThread.reset();
        }

        LOGINFO("Deinitializing SDL_ttf");
        TTF_Quit();

        LOGINFO("Shutting down ImGui renderer");
        ImGui_ImplSDLRenderer3_Shutdown();

        LOGINFO("Shutting down ImGui");
        ImGui_ImplSDL3_Shutdown();

        LOGINFO("Destroying ImGui context");
        ImGui::DestroyContext();

        Ctx::destroyContext(m_ctx);

        LOGINFO("Quitting...");
        SDL_Quit();
    }

    Utils::Signal App::init()
    {
        LOGINFO("Initializing App");

        if (initSDL() == Utils::Signal::Error)
        {
            LOGERR("SDL could not be initialized");
            return Utils::Signal::SDLInitError;
        }

        if (initFont() == Utils::Signal::Error)
        {
            LOGERR("Font not found");
            return Utils::Signal::FontLoadError;
        }

        if (initAudio() == Utils::Signal::Error)
        {
            LOGERR("Audio could not be initialized");
            return Utils::Signal::AudioInitError;
        }

        if (initImGui() == Utils::Signal::Error)
        {
            LOGERR("Could not initialize ImGui");
            return Utils::Signal::ImGuiInitError;
        }

        LOGINFO("Creating SortView");
        m_sortView = std::make_unique<Renderer::SortView>(shared_from_this(), m_UI.getUIState());

        LOGINFO("Creating Registry");
        m_sortRegistry = Core::SortRegistry(shared_from_this());
        m_sortRegistry.registerAllSorts();

        return Utils::Signal::Success;
    }

    Utils::Signal App::initSDL()
    {
        LOGINFO("Initializing SDL");

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            LOGERR("Could not initialize SDL");
            return Utils::Signal::Error;
        }
        LOGINFO("SDL initialized successfully");

        m_ctx = Core::Ctx::createContext(1920.0f, 1080.0f, 240);

        LOGINFO("Setting render parameters");
        SDL_SetRenderDrawColor(m_ctx->renderer, 0x0, 0x0, 0x0, 0x0);
        SDL_SetRenderDrawBlendMode(m_ctx->renderer, SDL_BLENDMODE_BLEND);

        LOGINFO("Clearing window");
        SDL_RenderClear(m_ctx->renderer);

        return Utils::Signal::Success;
    }

    Utils::Signal App::initFont()
    {
        LOGINFO("Loading font");
        TTF_Init();

        if (m_ctx->createFont("/res/font.ttf") == Utils::Signal::Error)
        {
            return Utils::Signal::Error;
        }

        return Utils::Signal::Success;
    }

    Utils::Signal App::initAudio()
    {
        LOGINFO("Initializing audio subsystem");
        m_soundEngine = SoundEngine::get();
        if (m_soundEngine->init() == Utils::Signal::Error)
        {
            LOGERR("Sound could not be initialized");
            if (m_soundEngine->alGetLastError() != AL_NO_ERROR)
            {
                LOGERR(
                    "Error playing audio with code: "
                    << m_soundEngine->alErrorString(m_soundEngine->alGetLastError()) << "("
                    << m_soundEngine->alGetLastError() << ")"
                );
                return Utils::Signal::Error;
            }
        }
        LOGINFO("Audio subsystem initialized successfully");
        return Utils::Signal::Success;
    }

    Utils::Signal App::initImGui()
    {
        LOGINFO("Setting up GUI");

        LOGINFO("Setting up ImGui context");
        IMGUI_CHECKVERSION();
        ImGuiContext* imCtx = ImGui::CreateContext();
        ImGui::SetCurrentContext(imCtx);

        LOGINFO("Configuring ImGui io");
        configureIO();

        LOGINFO("Setting ImGui styling");
        ImGuiStyle& style = ImGui::GetStyle();
        setStyle(style);

        LOGINFO("Setting up ImGui renderer");
        if (!ImGui_ImplSDL3_InitForSDLRenderer(m_ctx->window, m_ctx->renderer))
        {
            return Utils::Signal::Error;
        }

        LOGINFO("Initializing ImGui SDL renderer");
        if (!ImGui_ImplSDLRenderer3_Init(m_ctx->renderer)) { return Utils::Signal::Error; }

        m_UI = Renderer::UI(shared_from_this());

        return Utils::Signal::Success;
    }

    void App::run()
    {
        LOGINFO("Initializing sorter");
        if (auto* entry = m_sortRegistry.get("bubble"))
        {
            constexpr std::uint64_t defaultSize = 512;
            m_sorter                            = entry->factory();
            m_sorter->setLength(defaultSize);
        }

        LOGINFO("Creating audio thread");

        startAudioThread();

        LOGINFO("Starting main loop");
        SDL_PollEvent(&m_event);

        while (true)
        {
            if (SDL_PollEvent(&m_event))
            {
                SDL_ConvertEventToRenderCoordinates(m_ctx->renderer, &m_event);

                ImGui_ImplSDL3_ProcessEvent(&m_event);
                if (m_event.type == SDL_EVENT_QUIT)
                {
                    m_sorter->getFlags().setFlags(Sort::FlagGroup::Quit);

                    Utils::terminateThread(sortThread);

                    LOGINFO("Exit signal recieved");
                    break;
                }
            }

            auto start = std::chrono::high_resolution_clock::now();

            m_sortView->update(m_UI.getUIState());

            switch (m_UI.renderUI())
            {
                case Utils::Signal::StopSort: {
                    m_sorter->getFlags().reset();
                    break;
                }
                case Utils::Signal::CloseApp: {
                    m_sorter->timer.end();
                    m_sorter->realTimer.end();
                    return;
                }
                [[likely]] case Utils::Signal::Success:
                    [[fallthrough]];
                default:
                    break;
            }

            SDL_RenderPresent(m_ctx->renderer);

            std::chrono::duration<double, std::milli>
                   elapsed = std::chrono::high_resolution_clock::now() - start;

            double delay   = m_ctx->getFrameTime() - elapsed.count();

            if (delay > 1.5) { SDL_Delay(static_cast<Uint32>(delay - 1.0)); }

            while (true)
            {
                auto now = std::chrono::high_resolution_clock::now();

                if (std::chrono::duration<double, std::milli>(now - start).count()
                    >= m_ctx->getFrameTime())
                {
                    break;
                }
            }
        }
    }

    void App::startAudioThread()
    {
        m_audioThread = std::make_optional<std::thread>(
            [this]()
            {
                while (true)
                {
                    if (!m_sorter)
                    {
                        std::this_thread::sleep_for(100ms);
                        continue;
                    }

                    if (m_sorter->getFlags().wantClose) { break; }

                    constexpr float sec  = 0.04f;
                    constexpr float base = 100.f;
                    constexpr float min  = 100.f;
                    constexpr float max  = 800.f;

                    float           freq = 0.f;

                    if (m_sorter->elems.empty() || m_sorter->getFirst() >= m_sorter->elems.size())
                    {
                        std::this_thread::sleep_for(100ms);
                        continue;
                    }

                    freq = std::clamp<float>(
                        m_sorter->elems[m_sorter->getFirst()] * (m_ctx->winHeight / static_cast<float>(m_sorter->elems.size())) + base, min, max
                    );

                    if (m_sorter->getFlags().isSorting || m_sorter->getFlags().isShuffling || m_sorter->getFlags().isChecking)
                    {
                        if (m_soundEngine->load(sec, freq) == Utils::Signal::Error)
                        {
                            LOGERR("Could not load audio buffer");
                            if (m_soundEngine->alGetLastError() != AL_NO_ERROR)
                            {
                                LOGERR(
                                    "Error loading audio with code: "
                                    << m_soundEngine->alErrorString(m_soundEngine->alGetLastError())
                                    << "(" << m_soundEngine->alGetLastError() << ")"
                                );
                                return;
                            }
                        }

                        if (m_soundEngine->play() == Utils::Signal::Error)
                        {
                            LOGERR("Could not play audio buffer");
                            if (m_soundEngine->alGetLastError() != AL_NO_ERROR)
                            {
                                LOGERR(
                                    "Error playing audio with code: "
                                    << m_soundEngine->alErrorString(m_soundEngine->alGetLastError())
                                    << "(" << m_soundEngine->alGetLastError() << ")"
                                );
                                return;
                            }
                        }
                    }

                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(static_cast<int>(sec * 1000))
                    );
                }
            }
        );
    }

    void App::setStyle(ImGuiStyle& t_style) const noexcept
    {
#define IMGUI_COLOR_STYLE(param) t_style.Colors[ImGuiCol_##param]

        constexpr float WINDOW_PADDING          = 15.0f;
        constexpr float WINDOW_ROUNDING         = 3.0f;
        constexpr float FRAME_PADDING           = 5.0f;
        constexpr float FRAME_ROUNDING          = 3.0f;
        constexpr float INDENT_SPACING          = 25.0f;
        constexpr float SCROLLBAR_SIZE          = 15.0f;
        constexpr float SCROLLBAR_ROUNDING      = 3.0f;
        constexpr float GRAB_MIN_SIZE           = 5.0f;
        constexpr float GRAB_ROUNDING           = 3.0f;

        t_style.WindowPadding                   = ImVec2(WINDOW_PADDING, WINDOW_PADDING);
        t_style.WindowRounding                  = WINDOW_ROUNDING;
        t_style.FramePadding                    = ImVec2(FRAME_PADDING, FRAME_PADDING);
        t_style.FrameRounding                   = FRAME_ROUNDING;
        t_style.IndentSpacing                   = INDENT_SPACING;
        t_style.ScrollbarSize                   = SCROLLBAR_SIZE;
        t_style.ScrollbarRounding               = SCROLLBAR_ROUNDING;
        t_style.GrabMinSize                     = GRAB_MIN_SIZE;
        t_style.GrabRounding                    = GRAB_ROUNDING;

        IMGUI_COLOR_STYLE(Text)                 = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        IMGUI_COLOR_STYLE(TextDisabled)         = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        IMGUI_COLOR_STYLE(WindowBg)             = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        IMGUI_COLOR_STYLE(PopupBg)              = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        IMGUI_COLOR_STYLE(Border)               = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
        IMGUI_COLOR_STYLE(BorderShadow)         = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        IMGUI_COLOR_STYLE(FrameBg)              = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        IMGUI_COLOR_STYLE(FrameBgHovered)       = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        IMGUI_COLOR_STYLE(FrameBgActive)        = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        IMGUI_COLOR_STYLE(TitleBg)              = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        IMGUI_COLOR_STYLE(TitleBgCollapsed)     = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
        IMGUI_COLOR_STYLE(TitleBgActive)        = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        IMGUI_COLOR_STYLE(MenuBarBg)            = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        IMGUI_COLOR_STYLE(ScrollbarBg)          = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        IMGUI_COLOR_STYLE(ScrollbarGrab)        = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        IMGUI_COLOR_STYLE(ScrollbarGrabHovered) = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        IMGUI_COLOR_STYLE(ScrollbarGrabActive)  = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        IMGUI_COLOR_STYLE(CheckMark)            = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        IMGUI_COLOR_STYLE(SliderGrab)           = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        IMGUI_COLOR_STYLE(SliderGrabActive)     = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        IMGUI_COLOR_STYLE(Button)               = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        IMGUI_COLOR_STYLE(ButtonHovered)        = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        IMGUI_COLOR_STYLE(ButtonActive)         = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        IMGUI_COLOR_STYLE(Header)               = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        IMGUI_COLOR_STYLE(HeaderHovered)        = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        IMGUI_COLOR_STYLE(HeaderActive)         = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        IMGUI_COLOR_STYLE(ResizeGrip)           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        IMGUI_COLOR_STYLE(ResizeGripHovered)    = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        IMGUI_COLOR_STYLE(ResizeGripActive)     = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        IMGUI_COLOR_STYLE(PlotLines)            = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        IMGUI_COLOR_STYLE(PlotLinesHovered)     = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        IMGUI_COLOR_STYLE(PlotHistogram)        = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        IMGUI_COLOR_STYLE(PlotHistogramHovered) = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        IMGUI_COLOR_STYLE(TextSelectedBg)       = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

#undef IMGUI_COLOR_STYLE
    }

    void App::configureIO() noexcept
    {
        ImGuiIO& io = ImGui::GetIO();

        LOGINFO("Setting ImGui IO flags");

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    }
}  // namespace Core
