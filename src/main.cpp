#define SDL_MAIN_HANDLED
#include "core/app.h"
#include "core/app_ctx.h"
#include "core/logging/logging.h"

int main()
{
    std::shared_ptr<Core::App>
        app = std::make_shared<Core::App>();  // Create a unique pointer to App

    if (app->init() != Utils::Signal::Success) { return EXIT_FAILURE; }

    LOGINFO("App initialized successfully");
    app->run();

    return EXIT_SUCCESS;
}
