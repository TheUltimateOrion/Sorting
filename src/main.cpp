#define SDL_MAIN_HANDLED 1

#include "core/app.h"
#include "core/logging/logging.h"
#include "renderer/context.h"

#include <SDL3/SDL_main.h>

#include <memory>

#include <cstdlib>

int main()
{
    SDL_SetMainReady();
    std::shared_ptr<Core::App>
        app = std::make_shared<Core::App>();  // Create a unique pointer to App

    if (app->init() != Utils::Signal::Success) { return EXIT_FAILURE; }

    LOGINFO("App initialized successfully");
    app->run();

    return EXIT_SUCCESS;
}
