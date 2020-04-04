#pragma once

#include "memory.hpp"
#include "gapi.hpp"
#include "storage.hpp"
#include "system_demo.hpp"
#include "system_render.hpp"

enum class Room {
    titleScreen,
    mainMenu,
    debugLevel,
};

class Game {
public:
    // Times
    float currentTime = 0.f;
    float lastTime = 0.f;
    float frameTime = 0.f;
    float deltaTime = 0.f;
    const float partTime = 1000.f / 60.f;
    int frames = 0;
    int fps = 0;

    //

    RegionMemoryBuffer rootMemoryBuffer;
    GameMemory memory;
    Storage storage;
    Room room = Room::debugLevel;

    // Systems
    DemoSystem demoSystem;
    RenderSystem renderSystem;

    // Methods
    void init();

    void afterInit();

    void gameMainLoop(Platform platform, Window window);

private:
    void onRender();

    void onProgress(const float deltaTime);
};
