#pragma once

class App;

namespace AppCtx {
    extern std::unique_ptr<App> g_app; // Declare app as a unique pointer

    constexpr float kWinWidth = 1920.0f;
    constexpr float kWinHeight = 1080.0f;
    
    inline double getTimestamp() {
        static auto start = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - start).count();
    }
}