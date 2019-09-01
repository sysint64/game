#include "game.hpp"

#include "stdio.h"
#include "platform.hpp"
#include "memory.hpp"

bool initGameState() {
    auto bufferResult = allocMemoryBuffer(gigabytes(1));

    if (resultHasError(bufferResult)) {
        puts(bufferResult.error.message);
        return false;
    }

    gameState.rootMemoryBuffer = bufferResult.payload;
    gameState.totalMemory.currentBuffer = &gameState.rootMemoryBuffer;

    return true;
}
