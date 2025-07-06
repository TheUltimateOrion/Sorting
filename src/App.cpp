#include "App.h"
#include "renderer/Sound.h"
#include "sort/Sort.h"
#include "sort/BubbleSort.h"

App::App() noexcept
{
	this->categories = {"Exchange", "Distribution", "Insertion", "Merge", "Select"};
    this->items = {
		{"BubbleSort", "QuickSort", "CombSort", },
		{"RadixLSDSort", "PigeonHoleSort", "GravitySort", "BogoSort"},
		{"InsertionSort"},
		{"MergeSort"},
		{"SelectionSort"}
	};

    this->displayTypes = {"Bar", "Dot", "Rainbow Rectangle", "Circle", "Circle Dot", "Disparity Circle", "Spiral", "Spiral Dot"};

    snd = SoundEngine::get();
}

App::~App()
{    
    LOGINFO("Destroying font renderer");
    if (this->font) TTF_CloseFont(this->font);
    TTF_Quit();

    LOGINFO("Shutting down ImGui renderer");
    ImGui_ImplSDLRenderer3_Shutdown();

    LOGINFO("Shutting down ImGui");
    ImGui_ImplSDL3_Shutdown();

    LOGINFO("Destroying ImGui context");
    ImGui::DestroyContext();

    LOGINFO("Destroying SDL renderer");
    if (this->renderer) SDL_DestroyRenderer(this->renderer);

    LOGINFO("Destroying SDL window");
    if(this->window) SDL_DestroyWindow(this->window);

    LOGINFO("Quitting...");
    SDL_Quit();
    delete this->sortRenderer;
}

void App::_setupGUI()
{
    LOGINFO("Setting up ImGui context");
    IMGUI_CHECKVERSION();
    ImGuiContext* ctx= ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
	
    LOGINFO("Configuring ImGui io");
	this->io = &this->configureIO();

    LOGINFO("Setting ImGui styling");
    ImGuiStyle& style = ImGui::GetStyle();
	this->setStyle(style);
        
    LOGINFO("Setting up ImGui renderer");
    ImGui_ImplSDL3_InitForSDLRenderer(this->window, this->renderer);
    ImGui_ImplSDLRenderer3_Init(this->renderer);
    this->sortRenderer = new SortRenderer();
}

int App::init()
{
    LOGINFO("Initializing App");
    LOGINFO("Loading font");
    if(this->loadFont() < 0)
    {
        LOGERR("Font not found");
        return -1;
    }
    LOGINFO("Font loaded successfully");

    LOGINFO("Initializing sound subsystem");
    if (snd->init() < 0)
    {
        LOGERR("Sound could not be initialized");
        HANDLE_ERROR("Error initalizing", -1);
    }
    LOGINFO("Initialized sound subsystem successfully");

	// Window and Renderer Setups
    {    
        LOGINFO("Creating SDL Window");
        this->window = SDL_CreateWindow("Sorting Algorithms", WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_HIGH_PIXEL_DENSITY);
        LOGINFO("Creating SDL renderer");
        this->renderer = SDL_CreateRenderer(this->window, NULL);
        LOGINFO("Setting render parameters");
        SDL_SetRenderDrawColor(this->renderer, 0x0, 0x0, 0x0, 0x0);
		SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
        LOGINFO("Clearing window");
        SDL_RenderClear(this->renderer);
    }
    LOGINFO("Setting up GUI");
	this->_setupGUI();

    srand(time(NULL));
	return 0;
}

void App::run()
{
    LOGINFO("Generating array");

    constexpr int defaultSize = 512;
    data.resize(defaultSize);
    for (int index = 0; index < defaultSize; ++index)
        data[index] = index + 1;

    LOGINFO("Initializing sorter");
    this->sorter = std::make_shared<BubbleSort>(data);
    this->sorter->setSpeed(1);

    LOGINFO("Creating audio thread");
    std::thread audioThread([this]()
    {
        while(!this->sorter->wantClose) {
            constexpr float sec = 0.04f;
            constexpr int base = 100;
            constexpr int min = 100;
            constexpr int max = 800;

            int freq = this->data[this->current_element] * (WIN_HEIGHT / (float)this->data.size()) + base;
            freq = std::clamp(freq, min, max);
            if (this->sorter->isSorting || this->sorter->isShuffling){
                snd->load(sec, freq);
                HANDLE_ERROR("Error loading audio", );
                snd->play();
                HANDLE_ERROR("Error playing audio", );
                std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
            }
        }
    });
    audioThread.detach();

    LOGINFO("Starting main loop");
    SDL_PollEvent(&this->event);
    while(1)
    {
        this->sortRenderer->update(this->data, 1, 1);
        if (this->event.type == SDL_EVENT_QUIT || this->sorter->wantClose) {
            LOGINFO("Exit signal recieved");
            break;
        }
    }
}

void App::calculateDeltaTime() noexcept
{
   LAST = NOW;
   NOW = SDL_GetPerformanceCounter();
   deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
}

int App::loadFont() {
    TTF_Init();
    std::string basePath{SDL_GetBasePath()};
    constexpr const char* relPath = "/res/font.ttf";
    font = TTF_OpenFont((basePath + relPath).c_str(), 12);
    if (font == NULL) {
        return -1;
    }
    return 0;
}

void App::setStyle(ImGuiStyle& style) const noexcept
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

    style.WindowPadding = ImVec2(WINDOW_PADDING, WINDOW_PADDING);
    style.WindowRounding = WINDOW_ROUNDING;
    style.FramePadding = ImVec2(FRAME_PADDING, FRAME_PADDING);
    style.FrameRounding = FRAME_ROUNDING;
    style.IndentSpacing = INDENT_SPACING;
    style.ScrollbarSize = SCROLLBAR_SIZE;
    style.ScrollbarRounding = SCROLLBAR_ROUNDING;
    style.GrabMinSize = GRAB_MIN_SIZE;
    style.GrabRounding = GRAB_ROUNDING;
 
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
    (void)io;
    LOGINFO("Setting io flags");
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    return io;
}