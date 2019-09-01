#pragma once

#include <SDL2/SDL.h>

struct Platform {
    SDL_Window* window;
    SDL_GLContext glContext;
};
