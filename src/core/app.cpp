#include "core/app.h"

#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>

#include "core/logging/logging.h"
#include "sort/exchange/bubble.h"
#include "utils/common.h"
#include "core/platform/dpi.h"

using namespace std::literals::chrono_literals;

namespace Core 
{
    App::App() noexcept :   m_UI(nullptr),
                            currentCategory(Sort::Category::Exchange), 
                            currentDisplayType(Renderer::DisplayType::Bar),
                            sortRegistry(nullptr)
    {
        categories = {"Exchange", "Distribution", "Insertion", "Merge", "Select"};
        displayTypes = {"Bar", "Dot", "Rainbow Rectangle", "Circle", "Circle Dot", "Disparity Circle", "Spiral", "Spiral Dot"};
    }

    App::~App()
    {   
        if (sortThread.has_value()) 
        {
            if (sortThread->joinable())
            {
                LOGINFO("Joining and Destroying Sorting Thread");
                sortThread->join();
            }
            sortThread.reset();
        }

        if (m_audioThread.has_value()) 
        {
            if (m_audioThread->joinable())
            {
                LOGINFO("Joining Audio Thread");
                m_audioThread->join();
            }
            m_audioThread.reset();
        }

        if (font) 
        {
            LOGINFO("Destroying font renderer");
            TTF_CloseFont(font);
        }
        TTF_Quit();

        LOGINFO("Shutting down ImGui renderer");
        ImGui_ImplSDLRenderer3_Shutdown();

        LOGINFO("Shutting down ImGui");
        ImGui_ImplSDL3_Shutdown();

        LOGINFO("Destroying ImGui context");
        ImGui::DestroyContext();

        Ctx::destroyContext(ctx);

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
        
        if(initFont() == Utils::Signal::Error)
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
        m_sortView = std::make_unique<Renderer::SortView>(shared_from_this());
        sortRegistry = Core::SortRegistry(shared_from_this());
        sortRegistry.registerAllSorts();
        
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
        
        ctx = Core::Ctx::createContext(1920.0f, 1080.0f, 240);

        LOGINFO("Setting render parameters");
        SDL_SetRenderDrawColor(ctx->renderer, 0x0, 0x0, 0x0, 0x0);
        SDL_SetRenderDrawBlendMode(ctx->renderer, SDL_BLENDMODE_BLEND);

        LOGINFO("Clearing window");
        SDL_RenderClear(ctx->renderer);

        return Utils::Signal::Success;
    }

    Utils::Signal App::initFont() 
    {
        LOGINFO("Loading font");
        TTF_Init();
        std::string basePath{SDL_GetBasePath()};
        constexpr const char* relPath = "/res/font.ttf";
        font = TTF_OpenFont((basePath + relPath).c_str(), 12);

        if (font == NULL)
        {
            LOGINFO("Font failed to load: '" << (basePath + relPath) << "' is not a font or does not exist");
            return Utils::Signal::Error;
        }
        LOGINFO("Font loaded successfully from " << (basePath + relPath));

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
                LOGERR("Error playing audio with code: " << m_soundEngine->alErrorString(m_soundEngine->alGetLastError()) << "(" << m_soundEngine->alGetLastError() << ")");
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
        m_io = &configureIO();

        LOGINFO("Setting ImGui styling");
        ImGuiStyle& style = ImGui::GetStyle();
        setStyle(style);

        LOGINFO("Setting up ImGui renderer");
        if (!ImGui_ImplSDL3_InitForSDLRenderer(ctx->window, ctx->renderer))
        {
            return Utils::Signal::Error;
        }

        LOGINFO("Initializing ImGui SDL renderer");
        if (!ImGui_ImplSDLRenderer3_Init(ctx->renderer)) 
        {
            return Utils::Signal::Error;
        }

        m_UI = Renderer::UI(shared_from_this());

        // std::string fontPath = std::string(SDL_GetBasePath()) + "res/font.ttf";
        // float scale = ctx->dpi.scaleX > 0 ? ctx->dpi.scaleX : 2.0f;
        // float fontSize = 12.0f * scale;

        // ImFont* imguiFont = m_io->Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize);
        // if (!imguiFont) {
        //     LOGERR("Failed to load ImGui font");
        //     return Utils::Signal::Error;
        // }
        // ImGui_ImplSDLRenderer3_CreateFontsTexture();

        return Utils::Signal::Success;
    }

    void App::run()
    {
        LOGINFO("Initializing sorter");
        if (auto* entry = sortRegistry.get("bubble"))
        {
            constexpr uint64_t defaultSize = 512;
            sorter = entry->factory();
            sorter->setLength(defaultSize);
        }

        LOGINFO("Creating audio thread");

        startAudioThread();

        LOGINFO("Starting main loop");
        SDL_PollEvent(&event);

        while(true)
        {
            auto start = std::chrono::high_resolution_clock::now();
            m_sortView->update();

            switch (m_UI.renderUI()) 
            {
                case Utils::Signal::StopSort:
                {
                    sorter->isShuffling = false;
                    sorter->isSorting = false;
                    sorter->sorted = true;
                    sorter->wantStop = false;
                    break;
                }
                case Utils::Signal::CloseApp:
                    sorter->timer.end();
                    sorter->realTimer.end();
                    return;
                case Utils::Signal::Success:
                default: break;
            }

            SDL_RenderPresent(ctx->renderer);
            if(SDL_PollEvent(&event))
            {
                SDL_ConvertEventToRenderCoordinates(ctx->renderer, &event);

                ImGui_ImplSDL3_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                {
                    sorter->wantClose = true;
                    LOGINFO("Exit signal recieved");
                    break;
                }
            }

            std::chrono::duration<double, std::milli> elapsed = std::chrono::high_resolution_clock::now() - start;
            
            double delay = ctx->getFrameTime() - elapsed.count();

            if (delay > 1.5) 
            {
                SDL_Delay(static_cast<Uint32>(delay - 1.0));
            }

            while (true) 
            {
                auto now = std::chrono::high_resolution_clock::now();

                if (std::chrono::duration<double, std::milli>(now - start).count() >= ctx->getFrameTime()) 
                {
                    break;
                }
            }
        }
    }

    void App::startAudioThread()
    {
        m_audioThread = std::make_optional<std::thread>([this]()
        {
            while(!sorter->wantClose) 
            {
                constexpr float sec = 0.04f;
                constexpr int base = 100;
                constexpr int min = 100;
                constexpr int max = 800;

                int freq = 0;

                {
                    std::scoped_lock<std::mutex> lock{sorter->mutex};

                    if (sorter->elems.empty() || sorter->getFirst() >= sorter->elems.size()) 
                    {
                        std::this_thread::sleep_for(100ms);
                        continue;  // skip this iteration;
                    }

                    freq = sorter->elems[sorter->getFirst()] * (ctx->winHeight / static_cast<float>(sorter->elems.size())) + base;
                }

                freq = std::clamp(freq, min, max);

                if (sorter->isSorting || sorter->isShuffling || sorter->isChecking) 
                {
                    if (m_soundEngine->load(sec, freq) == Utils::Signal::Error)
                    {
                        LOGERR("Could not load audio buffer");
                        if (m_soundEngine->alGetLastError() != AL_NO_ERROR) 
                        {
                            LOGERR("Error loading audio with code: " << m_soundEngine->alErrorString(m_soundEngine->alGetLastError()) << "(" << m_soundEngine->alGetLastError() << ")");
                            return;
                        }
                    }

                    if (m_soundEngine->play() == Utils::Signal::Error)
                    {
                        LOGERR("Could not play audio buffer");
                        if (m_soundEngine->alGetLastError() != AL_NO_ERROR) 
                        {
                            LOGERR("Error playing audio with code: " << m_soundEngine->alErrorString(m_soundEngine->alGetLastError()) << "(" << m_soundEngine->alGetLastError() << ")");
                            return;
                        }
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sec * 1000)));
                }
            }
        });
    }

    void App::setStyle(ImGuiStyle& t_style) const noexcept
    {
        constexpr float WINDOW_PADDING = 15.0f;
        constexpr float WINDOW_ROUNDING = 3.0f;
        constexpr float FRAME_PADDING = 5.0f;
        constexpr float FRAME_ROUNDING = 3.0f;
        constexpr float INDENT_SPACING = 25.0f;
        constexpr float SCROLLBAR_SIZE = 15.0f;
        constexpr float SCROLLBAR_ROUNDING = 3.0f;
        constexpr float GRAB_MIN_SIZE = 5.0f;
        constexpr float GRAB_ROUNDING = 3.0f;

        t_style.WindowPadding = ImVec2(WINDOW_PADDING, WINDOW_PADDING);
        t_style.WindowRounding = WINDOW_ROUNDING;
        t_style.FramePadding = ImVec2(FRAME_PADDING, FRAME_PADDING);
        t_style.FrameRounding = FRAME_ROUNDING;
        t_style.IndentSpacing = INDENT_SPACING;
        t_style.ScrollbarSize = SCROLLBAR_SIZE;
        t_style.ScrollbarRounding = SCROLLBAR_ROUNDING;
        t_style.GrabMinSize = GRAB_MIN_SIZE;
        t_style.GrabRounding = GRAB_ROUNDING;
    
        STYLESET(Text)= ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
        STYLESET(TextDisabled) = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        STYLESET(WindowBg) = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        STYLESET(PopupBg)= ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        STYLESET(Border) = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
        STYLESET(BorderShadow) = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
        //style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        
        STYLESET(FrameBg) = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        STYLESET(FrameBgHovered) = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        STYLESET(FrameBgActive) = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        STYLESET(TitleBg) = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        STYLESET(TitleBgCollapsed) = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
        STYLESET(TitleBgActive) = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
        STYLESET(MenuBarBg) = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        //style->Colors[ImGuiCol_MenuBarBg) = ImVec4(0.15f, 0.135f, 0.18f, 1.00f);
        STYLESET(ScrollbarBg) = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        STYLESET(ScrollbarGrab) = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        STYLESET(ScrollbarGrabHovered) = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        STYLESET(ScrollbarGrabActive) = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        STYLESET(CheckMark) = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        STYLESET(SliderGrab) = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
        STYLESET(SliderGrabActive) = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        STYLESET(Button) = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        STYLESET(ButtonHovered) = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
        STYLESET(ButtonActive) = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        STYLESET(Header) = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
        STYLESET(HeaderHovered) = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        STYLESET(HeaderActive) = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        STYLESET(ResizeGrip) = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        STYLESET(ResizeGripHovered) = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
        STYLESET(ResizeGripActive) = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
        STYLESET(PlotLines) = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        STYLESET(PlotLinesHovered) = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        STYLESET(PlotHistogram) = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
        STYLESET(PlotHistogramHovered) = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
        STYLESET(TextSelectedBg) = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    }

    ImGuiIO& App::configureIO() noexcept
    {
        ImGuiIO& io = ImGui::GetIO();
        LOGINFO("Setting ImGui IO flags");
        
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        return io;
    }

    float App::getFramerate() const 
    {
        return m_io->Framerate;
    }
} // namespace Core