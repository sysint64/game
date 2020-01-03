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
    float currentTime = 0.f;
    float lastTime = 0.f;
    float frameTime = 0.f;
    float deltaTime = 0.f;

    const float partTime = 1000.f / 60.f;
    int frames = 0;
    int fps = 0;

    MemoryBuffer rootMemoryBuffer;
    Memory totalMemory;
    Room room = Room::debugLevel;

    QuadGeometry testSprite;
    Transforms2D testSpriteTransforms;
    ShaderProgram spriteShader;
    u32 spriteShaderLocationMVP;
    u32 spriteShaderLocationTexture;
    CameraMatrices cameraMatrices;
    Texture2D spriteTexture;
    glm::mat4 testSpriteModelMatrix;
    glm::mat4 testSpriteMVPMatrix;

    OthroCameraTransforms cameraTransform = {
        .viewportSize = glm::vec2(1024, 768),
        .position = glm::vec2(0, 0),
        .zoom = 1.f
    };
};

static GameState gameState;

bool initGameState();

bool afterInitGameState();

void gameMainLoop(Platform platform, Window window);
