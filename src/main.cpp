#include "App.h"

App* app;

int main(int argc, char const *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) { 
        fprintf(stderr, "Could not initialize SDL: %s.\n", SDL_GetError());
        return -1;
    }

    app = new App();

    if (app->init() < 0) return -1;

    app->run();

    delete app;
    return EXIT_SUCCESS;
}