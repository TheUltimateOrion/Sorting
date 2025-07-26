#pragma once
#include <memory>
#include <chrono>

#include "registry/sort_registry.h"

namespace Core 
{
    class App;
}

namespace AppCtx 
{
    extern std::unique_ptr<Core::App> g_app; // Declare app as a unique pointer
    extern Core::SortRegistry g_sortRegistry; // Global sort registry

    constexpr float kWinWidth = 1920.0f;
    constexpr float kWinHeight = 1080.0f;
    
    constexpr double kFrameTime = 1000.0 / 240.0; // 120 FPS
    
    inline int32_t g_sortRadix = 2;

    inline double getTimestamp() 
    {
        static auto start = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - start).count();
    }
}