#pragma once

#include <windows.h>

#define SDL_MAIN_HANDLED
#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_sdl3.h>
#include <imgui/backend/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <bits/stdc++.h>

#define LOGICAL_WIDTH 512.0f
#define WIN_WIDTH  LOGICAL_WIDTH
#define WIN_HEIGHT LOGICAL_WIDTH

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;