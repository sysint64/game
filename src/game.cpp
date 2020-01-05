#include "game.hpp"

#include <stdio.h>
#include "platform.hpp"
#include "memory.hpp"
#include "assets.hpp"
#include "gapi.hpp"

void Game::init() {
    auto bufferResult = createRegionMemoryBuffer(megabytes(75));
    rootMemoryBuffer = resultUnwrap(bufferResult);

    regionMemoryBufferAddRegion(&rootMemoryBuffer, &memory.eternityBuffer, megabytes(10));
    regionMemoryBufferAddRegion(&rootMemoryBuffer, &memory.assetsBuffer, megabytes(40));
    regionMemoryBufferAddRegion(&rootMemoryBuffer, &memory.frameBuffer, megabytes(20));
    regionMemoryBufferAddRegion(&rootMemoryBuffer, &memory.tmpBuffer, megabytes(5));
}

void Game::afterInit() {
    demoSystem.init(&memory);
}

void Game::gameMainLoop(Platform platform, Window window) {
    currentTime = platformGetTicks();
    deltaTime = lastTime - currentTime;

    if (currentTime >= lastTime + partTime) {
        onProgress(deltaTime);
        onRender();
        frames += 1;
        lastTime = currentTime;
        gapiSwapWindow(platform, window);
    }

    if (currentTime >= frameTime + 1000.0) {
        frameTime = currentTime;
        fps = frames;
        frames = 1;

        printf("FPS: %d\n", fps);
    }
}

void Game::onRender() {
    gapiClear(150.0f/255.0f, 150.0f/255.0f, 150.0f/255.0f);
    demoSystem.onRender();
}

void Game::onProgress(const float deltaTime) {
    demoSystem.onProgress(deltaTime);
}
