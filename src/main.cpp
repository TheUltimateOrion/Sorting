#include "main.h"

#include "sort/SelectionSort.h"
#include "sort/BubbleSort.h"
#include "sort/InsertionSort.h"
#include "sort/QuickSort.h"
#include "sort/PigeonHoleSort.h"
#include "sort/GravitySort.h"

SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
TTF_Font *font;

int main(int argc, char const *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        return -1;
    }
    TTF_Init();
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string str = std::string(buffer).substr(0, pos);
    str += "\\res\\font.ttf";
    font = TTF_OpenFont(str.c_str(), 12);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
        
    window = SDL_CreateWindow("Sorting Algorithms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_WIDTH);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    IMGUI_CHECKVERSION();
    ImGuiContext* ctx= ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    srand(time(NULL));

    std::vector<int> nums(LOGICAL_WIDTH);
    for (int index = 0; index < LOGICAL_WIDTH; index++)
        nums[index] = index + 1;
    std::shuffle(std::begin(nums), std::end(nums), std::default_random_engine(0));
    
    QuickSort sorter(nums, io);
    SDL_PollEvent(&event);
    while(1)
    {
        
        if (!(sorter.sorted))
            sorter.sort(1);
            if (event.type == SDL_QUIT)
                break;
        sorter.render(sorter.elems, 1, 1, sorter.speed);
        SDL_Delay(1);
    }
    //std::cout << func_time(bubble_sort, nums) << " seconds" << std::endl;
    TTF_CloseFont(font);
    TTF_Quit();
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}