#include "utils/renderer.h"

namespace Utils {
    SDL_Color hsvToRgb(unsigned char t_hue, unsigned char t_saturation, unsigned char t_value) noexcept {
        SDL_Color rgb;
        unsigned char region, remainder, p, q, t;
        region = t_hue / 43;
        remainder = (t_hue - (region * 43)) * 6; 
        
        p = (t_value * (0xFF - t_saturation)) >> 8;
        q = (t_value * (0xFF - ((t_saturation * remainder) >> 8))) >> 8;
        t = (t_value * (0xFF - ((t_saturation * (0xFF - remainder)) >> 8))) >> 8;
        
        switch (region)
        {
            case 0:
                rgb.r = t_value; rgb.g = t; rgb.b = p;
                break;
            case 1:
                rgb.r = q; rgb.g = t_value; rgb.b = p;
                break;
            case 2:
                rgb.r = p; rgb.g = t_value; rgb.b = t;
                break;
            case 3:
                rgb.r = p; rgb.g = q; rgb.b = t_value;
                break;
            case 4:
                rgb.r = t; rgb.g = p; rgb.b = t_value;
                break;
            default:
                rgb.r = t_value; rgb.g = p; rgb.b = q;
                break;
        }
        
        return rgb;
    }
}