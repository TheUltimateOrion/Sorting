#pragma once
#define SDL_MAIN_HANDLED
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <bits/stdc++.h>
#include <windows.h>

#define LOGICAL_WIDTH 512
#define WIN_WIDTH  LOGICAL_WIDTH
#define WIN_HEIGHT LOGICAL_WIDTH

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << (double) clock() / 1000.0 << "s]: " << str << std::endl;