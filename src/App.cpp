#include "App.h"

App* app;
const char* alErrorString(int err) {
    switch (err) {
        case AL_NO_ERROR: return "AL_NO_ERROR";
        case ALC_INVALID_DEVICE: return "ALC_INVALID_DEVICE";
        case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT";
        case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
        case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
        /* ... */
        default:
            return "Unknown error code";
    }
}

App::App()
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
    //std::cout << func_time(bubble_sort, nums) << " seconds" << std::endl;
    TTF_CloseFont(this->font);
    TTF_Quit();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    delete snd;
    SDL_Quit();
}

void App::_setupGUI()
{
	// ImGui Context Setup
    IMGUI_CHECKVERSION();
    ImGuiContext* ctx= ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
	
	this->io = &app->configureIO();

    ImGuiStyle * style = &ImGui::GetStyle();
 
	this->setStyle(style);
        
    ImGui_ImplSDL2_InitForSDLRenderer(this->window, this->renderer);
    ImGui_ImplSDLRenderer2_Init(this->renderer);

    srand(time(NULL));
}

int App::init()
{
    if(this->loadFont() < 0)
    {
        fprintf(stderr, "error: font not found\n");
        return -1;
    }

    if (snd->init() < 0)
    {
        fprintf(stderr, "error: Sound could not be initialized\n");
        return -1;
    }

	// Window and Renderer Setups
    {    
        this->window = SDL_CreateWindow("Sorting Algorithms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetLogicalSize(this->renderer, LOGICAL_WIDTH, LOGICAL_WIDTH);
        SDL_SetRenderDrawColor(this->renderer, 0x0, 0x0, 0x0, 0x0);
		SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderClear(this->renderer);
    }
	this->_setupGUI();
	return 0;
}

void App::run()
{
    std::vector<int> nums(LOGICAL_WIDTH);
    for (int index = 0; index < nums.capacity(); index++)
        nums[index] = index + 1;

	this->sorter = new BubbleSort(nums, io);
    this->sorter->setSpeed(1);

    std::thread audioThread([this]()
    {
        while(!this->sorter->wantClose) {
            float sec = 0.05;
            int freq = this->sorter->elems[this->current_element] * (LOGICAL_WIDTH / (float)this->sorter->elems.size());
            if (this->sorter->isSorting || this->sorter->isShuffling){
                snd->load(sec, freq);
                snd->play();
                std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
            }
        }
    });
    audioThread.detach();

    SDL_PollEvent(&this->event);
    while(1)
    {        
        this->sorter->setLength(this->setLength);
        this->sorter->swaps = this->swaps;
        this->sorter->comparisions = this->comparisions;
        this->sorter->sortRenderer->update(this->sorter->elems, 1, 1);
        if (this->event.type == SDL_QUIT || this->sorter->wantClose)
            break;
        SDL_Delay(1);
    }
}

void App::calculateDeltaTime()
{
   LAST = NOW;
   NOW = SDL_GetPerformanceCounter();
   deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
}

int App::loadFont() {
    TTF_Init();
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string str = std::string(buffer).substr(0, pos);
    str += "\\res\\font.ttf";
    font = TTF_OpenFont(str.c_str(), 12);
    if (font == NULL) {
        return -1;
    }
    return 0;
}

void App::setStyle(ImGuiStyle* style)
{
    style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 3.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 3.0f;
	// style->ItemSpacing = ImVec2(12, 8);
	// style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 3.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
 
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.135f, 0.18f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}

ImGuiIO& App::configureIO()
{
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    return io;
}