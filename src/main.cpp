#define SDL_MAIN_HANDLED
#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"

std::unique_ptr<Core::App> AppCtx::g_app = nullptr; // Initialize the unique pointer
Core::SortRegistry AppCtx::g_sortRegistry; // Initialize the global sort registry

int main()
{
    AppCtx::g_app = std::make_unique<Core::App>(); // Create a unique pointer to App

    if (AppCtx::g_app->init() != Utils::Signal::Success) return EXIT_FAILURE;
    LOGINFO("App initialized successfully");
    AppCtx::g_app->run();

    return EXIT_SUCCESS;
}