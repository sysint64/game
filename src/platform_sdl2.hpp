#pragma once

#include "gapi.hpp"
#include <SDL2/SDL.h>

struct Platform {
    GApi gapi;
};

struct Window {
    SDL_Window* sdlWindow;
    GApiContext gapiContext;
};
