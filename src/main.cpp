#include "main.h"

#include "sort/BubbleSort.h"
#include "renderer/Renderer.h"

SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
TTF_Font *font;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double deltaTime = 0;

Mix_Chunk* sfx = NULL;

void calculateDeltaTime()
{
   LAST = NOW;
   NOW = SDL_GetPerformanceCounter();
   deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
}

int loadFonts() {
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

int loadSound()
{
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string path = std::string(buffer).substr(0, pos);
    path += "\\res\\snd.wav";

    sfx = Mix_LoadWAV(path.c_str());
    return 0;
}

ImGuiIO& configureIO()
{
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    return io;
}

int main(int argc, char const *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1) { 
        fprintf(stderr, "Could not initialize SDL: %s.\n", SDL_GetError());
        return -1;
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    if(loadFonts() < 0)
    {
        fprintf(stderr, "error: font not found\n");
        return -1;
    }

    if (loadSound() < 0)
    {
        fprintf(stderr, "error: sound not found\n");
        return -1;
    }

    // Window and Renderer Setups
    {    
        window = SDL_CreateWindow("Sorting Algorithms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_WIDTH);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }

    // ImGui Context Setup
    IMGUI_CHECKVERSION();
    ImGuiContext* ctx= ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    ImGuiIO& io = configureIO();

    ImGui::StyleColorsDark();
        
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    srand(time(NULL));

    std::vector<int> nums(LOGICAL_WIDTH);
    for (int index = 0; index < nums.capacity(); index++)
        nums[index] = index + 1;
    
    Sort* sorter = new BubbleSort(nums, io);
    sorter->setSpeed(1);

    SDL_PollEvent(&event);
    while(1)
    {
        sorter->setLength(setLength);
        SortRenderer::render(sorter, sorter->elems, 1, 1);
        if (sorter->wantBreak || event.type == SDL_QUIT)
            break;
        SDL_Delay(1);
    }

    delete sorter;
    Mix_FreeChunk(sfx); sfx = NULL;
    //std::cout << func_time(bubble_sort, nums) << " seconds" << std::endl;
    TTF_CloseFont(font);
    TTF_Quit();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    SDL_Quit();
    return 0;
}