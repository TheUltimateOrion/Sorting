#pragma once

#include <chrono>
#include <optional>
#include <thread>

#ifndef STYLESET
    #define STYLESET(param) t_style.Colors[ImGuiCol_##param]
#endif

namespace Utils
{
    enum Signal
    {
        ImGuiInitError = -0x06,
        AudioPlayError = -0x05,
        AudioInitError = -0x04,
        FontLoadError  = -0x03,
        SDLInitError   = -0x02,
        Error          = -0x01,
        Success        = 0x00,
        StopSort       = 0x01,
        CloseApp       = 0x02,
    };

    void terminateThread(std::optional<std::thread>& t_thread);
}  // namespace Utils
