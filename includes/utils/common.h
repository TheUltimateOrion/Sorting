#pragma once
#include <chrono>

#if defined(TESTING)
    #define LOCK_GUARD do {} while(0);
    #define HIGH_RES_WAIT(t) do {} while(0);
#else
    #define LOCK_GUARD std::lock_guard<std::mutex> lock(AppCtx::g_app->m_mutex)

    #define HIGH_RES_WAIT(t)\
        auto start = std::chrono::high_resolution_clock::now();\
        while (true) {\
            auto now = std::chrono::high_resolution_clock::now();\
            double elapsed = std::chrono::duration<double, std::milli>(now - start).count();\
            if (elapsed >= (t)) break;\
        }
#endif

#ifndef STYLESET
#define STYLESET(param) t_style.Colors[ImGuiCol_##param]
#endif


namespace utils {
    
};