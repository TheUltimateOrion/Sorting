#include "App.h"

int main(int argc, char const *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOGERR("Could not initialize SDL: " << SDL_GetError());
        return -1;
    }

    app = new App();

    if (app->init() < 0) return -1;

    app->run();

    delete app;
    return EXIT_SUCCESS;
}