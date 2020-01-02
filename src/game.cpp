#include "game.hpp"

#include "stdio.h"
#include "platform.hpp"
#include "memory.hpp"
#include "assets.hpp"
#include "gapi.hpp"
#include "gapi_geometries.hpp"

static void initSprite();

static void initShaders();

static void initTexture();

static void updateTestSprite(const float deltaTime);

static void updateCamera();

static void onRender();

static void onProgress(const float deltaTime);

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

bool afterInitGameState() {
    initSprite();
    initShaders();
    initTexture();
    return true;
}

static void initSprite() {
    gameState.testSprite.indicesBuffer = gapiCreateU32Buffer(&quadIndices[0], quadIndicesCount, false);
    gameState.testSprite.verticesBuffer = gapiCreateVec2fBuffer(&quadVertices[0], quadVerticesCount, false);
    gameState.testSprite.texCoordsBuffer = gapiCreateVec2fBuffer(&quadTexCoords[0], quadTexCoordsCount, false);
    gameState.testSprite.vao = gapiCreateVAO();

    gapiBindVAO(&gameState.testSprite.vao);
    gapiCreateVector2fVAO(&gameState.testSprite.verticesBuffer, 0);
    gapiCreateVector2fVAO(&gameState.testSprite.texCoordsBuffer, 1);
}

Shader shadersMemory[100000];
size_t shadersMemoryCursor = 0;

#define PUSH_SHADER(shader) \
    memcpy(&shadersMemory[shadersMemoryCursor], &shader, sizeof(Shader)); \
    shadersMemoryCursor += sizeof(Shader);

static void initShaders() {
    const Result<AssetData> fragmentShaderResult = platformLoadAssetData(
        AssetType::shader,
        "fragment_texture.glsl"
    );

    const Result<AssetData> vertexShaderResult = platformLoadAssetData(
        AssetType::shader,
        "vertex_transform.glsl"
    );

    const AssetData fragmentShaderData = resultUnwrap(fragmentShaderResult);
    const AssetData vertexShaderData = resultUnwrap(vertexShaderResult);

    const auto vertexShaderResult2 = gapiCreateShader(
        "vertex_transform",
        ShaderType::vertex,
        vertexShaderData
    );
    const auto fragmentShaderResult2 = gapiCreateShader(
        "fragment_texture",
        ShaderType::fragment,
        fragmentShaderData
    );

    const Shader vertexShader = resultUnwrap(vertexShaderResult2);
    const Shader fragmentShader = resultUnwrap(fragmentShaderResult2);

    PUSH_SHADER(vertexShader);
    PUSH_SHADER(fragmentShader);

    const auto res = gapiCreateShaderProgram(
        "test_shader_program",
        StaticArray<Shader>{
            .size = 2,
            .items = shadersMemory
        }
    );

    const ShaderProgram testShader = resultUnwrap(res);
}

static void initTexture() {
    const Result<AssetData> testTextureResult = platformLoadAssetData(
        AssetType::texture,
        "test.jpg"
    );

    const AssetData testTexture = resultUnwrap(testTextureResult);
    const Texture2DParameters params = {
        .minFilter = true,
        .magFilter = true,
    };
    gameState.spriteTexture = gapiCreateTexture2D(testTexture, params);
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
}

static void onProgress(const float deltaTime) {
    updateCamera();
    updateTestSprite(deltaTime);
}

static void updateCamera() {
    gameState.cameraMatrices = gapiCreateOrthoCameraMatrices(gameState.cameraTransform);
}

static void updateTestSprite(const float deltaTime) {
    gameState.testSpriteTransforms.position = glm::vec2(
        gameState.cameraTransform.viewportSize.x / 2.f,
        gameState.cameraTransform.viewportSize.y / 2.f
    );
    gameState.testSpriteTransforms.scaling = glm::vec2(430.0f, 600.0f);
    gameState.testSpriteTransforms.rotation += 0.25f * deltaTime;

    gameState.testSpriteModelMatrix = gapiCreate2DModelMatrix(gameState.testSpriteTransforms);
    gameState.testSpriteMVPMatrix = gameState.cameraMatrices.mvpMatrix * gameState.testSpriteModelMatrix;
}
