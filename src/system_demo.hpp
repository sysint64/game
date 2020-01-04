#pragma once

#include "gapi.hpp"
#include "memory.hpp"

class DemoSystem {
public:
    void init(GameMemory* memory);
    void onRender();
    void onProgress(float deltaTime);

private:
    GeometryBuffer quadIndicesBuffer;
    GeometryBuffer quadVerticesBuffer;
    GeometryBuffer quadTexCoordsBuffer;
    GeometryVAO quadVao;

    GameMemory* memory;

    Transforms2D testSpriteTransforms;
    Texture2D testSpriteTexture;
    glm::mat4 testSpriteModelMatrix;
    glm::mat4 testSpriteMVPMatrix;

    ShaderProgram spriteShader;
    u32 spriteShaderLocationMVP;
    u32 spriteShaderLocationTexture;

    CameraMatrices cameraMatrices;
    OthroCameraTransforms cameraTransform = {
        .viewportSize = glm::vec2(1024, 768),
        .position = glm::vec2(0, 0),
        .zoom = 1.f
    };

    void initSprite();
    void initShaders();
    void initTexture();
};
