#pragma once

#include <optional>
#include <string>
#include <thread>

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

    void        terminateThread(std::optional<std::thread>& t_thread);
    std::string demangleName(char const* name);
}  // namespace Utils
