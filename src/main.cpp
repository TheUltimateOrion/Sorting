#include "App.h"

App* app;

int main(int argc, char const *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) { 
        fprintf(stderr, "Could not initialize SDL: %s.\n", SDL_GetError());
        return -1;
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return -1;
    }

    app = new App();
    
    if (app->init() < 0)
        return -1;

    app->run();

    delete app;
    return 0;
}