#pragma once

#include "memory.hpp"
#include "gapi.hpp"

enum class Room {
    titleScreen,
    mainMenu,
    debugLevel,
};

struct QuadGeometry {
    GeometryBuffer indicesBuffer;
    GeometryBuffer verticesBuffer;
    GeometryBuffer texCoordsBuffer;
    GeometryVAO vao;
};

struct GameState {
    MemoryBuffer rootMemoryBuffer;
    Memory totalMemory;
    Room room = Room::debugLevel;

    QuadGeometry testSprite;
    ShaderProgram spriteShader;
    CameraMatrices cameraMatrices;
    OthroCameraTransforms cameraTransform = {
        .viewportSize = glm::vec2(1024, 768),
        .position = glm::vec2(0, 0),
        .zoom = 1.f
    };
};

static GameState gameState;

bool initGameState();

void gameMainLoop();
