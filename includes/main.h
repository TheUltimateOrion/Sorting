#pragma once
#define SDL_MAIN_HANDLED
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <queue>
#include <list>

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

extern Uint64 NOW;
extern Uint64 LAST;
extern double deltaTime;

extern Mix_Chunk* sfx;
extern int setLength;
extern unsigned int swaps;
extern unsigned int comparisions;

extern void calculateDeltaTime();