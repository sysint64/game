#pragma once

#include "memory.hpp"

enum class Room {
    titleScreen,
    mainMenu,
    debugLevel,
};

struct GameState {
    MemoryBuffer rootMemoryBuffer;
    Memory totalMemory;
    Room room = Room::debugLevel;
};

static GameState gameState;

bool initGameState();
