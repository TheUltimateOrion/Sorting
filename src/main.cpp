#define SDL_MAIN_HANDLED
#include "core/App.h"
#include "core/AppCtx.h"
#include "core/logging/Logging.h"

std::unique_ptr<App> AppCtx::g_app = nullptr; // Initialize the unique pointer

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        LOGERR("Could not initialize SDL: " << SDL_GetError());
        return -1;
    }

    AppCtx::g_app = std::make_unique<App>(); // Create a unique pointer to App

    if (AppCtx::g_app->init() < 0) return EXIT_FAILURE;
    LOGINFO("App initialized successfully");
    AppCtx::g_app->run();

    return EXIT_SUCCESS;
}