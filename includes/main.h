#pragma once
#define SDL_MAIN_HANDLED
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cmath>

#include <bits/stdc++.h>
#include <windows.h>
#include <time.h>

#define LOGICAL_WIDTH 512
#define WIN_WIDTH LOGICAL_WIDTH
#define WIN_HEIGHT LOGICAL_WIDTH

extern SDL_Renderer *renderer;
extern SDL_Window *window;
extern SDL_Event event;
extern TTF_Font *font;

static const char* current_item = NULL;
static bool pressed = false;