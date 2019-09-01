#pragma once

#include "gapi.hpp"
#include <SDL2/SDL.h>

struct Platform {
    SDL_Window* window;
    GApiContext gapiContext;
};
