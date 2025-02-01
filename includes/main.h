#pragma once

#define SDL_MAIN_HANDLED

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <thread>
#include <random>

#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <AL/al.h>
#include <AL/alc.h>



#define WIN_WIDTH  1920.0f
#define WIN_HEIGHT 1080.0f

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;