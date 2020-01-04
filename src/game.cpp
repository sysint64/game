#include "game.hpp"

#include <stdio.h>
#include "platform.hpp"
#include "memory.hpp"
#include "assets.hpp"
#include "gapi.hpp"

static void onRender();

static void onProgress(const float deltaTime);

bool initGameState() {
    auto bufferResult = createRegionMemoryBuffer(megabytes(75));
    gameState.rootMemoryBuffer = resultUnwrap(bufferResult);

    regionMemoryBufferAddRegion(&gameState.rootMemoryBuffer, &gameState.memory.eternityBuffer, megabytes(10));
    regionMemoryBufferAddRegion(&gameState.rootMemoryBuffer, &gameState.memory.assetsBuffer, megabytes(40));
    regionMemoryBufferAddRegion(&gameState.rootMemoryBuffer, &gameState.memory.frameBuffer, megabytes(20));
    regionMemoryBufferAddRegion(&gameState.rootMemoryBuffer, &gameState.memory.tmpBuffer, megabytes(5));

    return true;
}

bool afterInitGameState() {
    gameState.demoSystem.init(&gameState.memory);
    return true;
}

void gameMainLoop(Platform platform, Window window) {
    gameState.currentTime = platformGetTicks();
    gameState.deltaTime = gameState.lastTime - gameState.currentTime;

    if (gameState.currentTime >= gameState.lastTime + gameState.partTime) {
        onProgress(gameState.deltaTime);
        onRender();
        gameState.frames += 1;
        gameState.lastTime = gameState.currentTime;
        gapiSwapWindow(platform, window);
    }

    if (gameState.currentTime >= gameState.frameTime + 1000.0) {
        gameState.frameTime = gameState.currentTime;
        gameState.fps = gameState.frames;
        gameState.frames = 1;

        printf("FPS: %d\n", gameState.fps);
    }
}

static void onRender() {
    gapiClear(150.0f/255.0f, 150.0f/255.0f, 150.0f/255.0f);
    gameState.demoSystem.onRender();
}

static void onProgress(const float deltaTime) {
    gameState.demoSystem.onProgress(deltaTime);
}
