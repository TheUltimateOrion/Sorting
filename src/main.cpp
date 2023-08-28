#include "App.h"

#include "sort/BubbleSort.h"
#include "renderer/Renderer.h"

App* app;

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

    app = new App();

    if(app->loadFonts() < 0)
    {
        fprintf(stderr, "error: font not found\n");
        return -1;
    }

    if (app->loadSound() < 0)
    {
        fprintf(stderr, "error: sound not found\n");
        return -1;
    }

    // Window and Renderer Setups
    {    
        app->window = SDL_CreateWindow("Sorting Algorithms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetLogicalSize(app->renderer, LOGICAL_WIDTH, LOGICAL_WIDTH);
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 0);
        SDL_RenderClear(app->renderer);
    }

    // ImGui Context Setup
    IMGUI_CHECKVERSION();
    ImGuiContext* ctx= ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);

    ImGuiIO& io = app->configureIO();

    ImGuiStyle * style = &ImGui::GetStyle();
 
	app->setStyle(style);
        
    ImGui_ImplSDL2_InitForSDLRenderer(app->window, app->renderer);
    ImGui_ImplSDLRenderer2_Init(app->renderer);

    srand(time(NULL));

    std::vector<int> nums(LOGICAL_WIDTH);
    for (int index = 0; index < nums.capacity(); index++)
        nums[index] = index + 1;
    
    app->sorter = new BubbleSort(nums, io);
    app->sorter->setSpeed(1);

    SDL_PollEvent(&app->event);
    while(1)
    {
        app->sorter->setLength(app->setLength);
        app->sorter->swaps = app->swaps;
        app->sorter->comparisions = app->comparisions;
        SortRenderer::render(app->sorter, app->sorter->elems, 1, 1);
        if (app->event.type == SDL_QUIT)
            break;
        SDL_Delay(1);
    }
    delete app;
    return 0;
}