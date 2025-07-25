#pragma once
#include <chrono>

#if defined(TESTING)
    #define LOCK_GUARD do {} while(0);
    #define HIGH_RES_WAIT(t) do {} while(0);
#else
    #define LOCK_GUARD std::lock_guard<std::mutex> lock(AppCtx::g_app->m_mutex)

    #define HIGH_RES_WAIT(t)\
        auto start = std::chrono::high_resolution_clock::now();\
        while (true)\
        {\
            auto now = std::chrono::high_resolution_clock::now();\
            double elapsed = std::chrono::duration<double, std::milli>(now - start).count();\
            if (elapsed >= (t)) break;\
        }
#endif

#ifndef STYLESET
#define STYLESET(param) t_style.Colors[ImGuiCol_##param]
#endif


namespace Utils 
{
    enum Signal {
        ImGuiInitError = -0x06,
        AudioPlayError = -0x05,
        AudioInitError = -0x04,
        FontLoadError = -0x03,
        SDLInitError = -0x02,
        Error = -0x01,
        Success = 0x00,
        StopSort = 0x01,
        CloseApp = 0x02,
    };
}
