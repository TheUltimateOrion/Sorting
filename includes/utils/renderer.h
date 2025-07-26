#pragma once
#include <cmath>
#include <SDL3/SDL.h>

namespace Utils 
{
    constexpr float kRadiansPerDegree = M_PI / 180.0f;

    SDL_Color hsvToRgb(unsigned char t_hue, unsigned char t_saturation, unsigned char t_value) noexcept;
} // namespace Utils
