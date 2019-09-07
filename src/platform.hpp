#pragma once

struct Platform;

struct Window;

#ifdef PLATFORM_SDL2
#include "platform_sdl2.hpp"
#endif

#ifdef PLATFORM_GLFW
#include "platform_glfw.hpp"
#endif

#include "game_types.hpp"

Result<Platform> platformInit();

Result<Window> platformCreateWindow(Platform platform);

bool platformEventLoop(Platform platform, Window window);

float platformGetTicks();

void platformSwapWindow(Platform platform, Window window);

void platformDestroyWindow(Window window);

void platformShutdown(Platform platform);

void* platformAlloc(MemoryIndex size);
