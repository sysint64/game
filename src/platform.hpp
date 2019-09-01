#pragma once

struct Platform;

#ifdef PLATFORM_SDL2
#include "platform_sdl2.hpp"
#endif

#ifdef PLATFORM_GLFW
#include "platform_glfw.hpp"
#endif

#include "game_types.hpp"

Result<Platform> platformInit();

bool platformEventLoop(Platform platform);

float platformGetTicks();

void platformSwapWindow(Platform platform);

void platformShutdown(Platform platform);

void* platformAlloc(MemoryIndex size);
