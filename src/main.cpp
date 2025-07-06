#define SDL_MAIN_HANDLED
#include "App.h"

std::unique_ptr<App> app = nullptr; // Declare app as a unique pointer


int main(int argc, char **argv) // (int argc, char const *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOGERR("Could not initialize SDL: " << SDL_GetError());
        return -1;
    }

    app = std::make_unique<App>(); // Create a unique pointer to App

    if (app->init() < 0) return EXIT_FAILURE;

    app->run();

    return EXIT_SUCCESS;
}