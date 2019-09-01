#pragma once

#include "game_types.hpp"

struct Platform;

Result<Platform> platformInit();

bool platformEventLoop(Platform platform);

float platformGetTicks();

void platformSwapWindow(Platform platform);

void platformShutdown(Platform platform);

void* platformAlloc(MemoryIndex size);
