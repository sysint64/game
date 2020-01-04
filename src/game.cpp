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

static void renderSprite();

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
    initSprite();
    initShaders();
    initTexture();
    return true;
}

static void initSprite() {
    gameState.testSprite.indicesBuffer = gapiCreateU32Buffer(&quadIndices[0], quadIndicesCount, false);
    gameState.testSprite.verticesBuffer = gapiCreateVec2fBuffer(&centeredQuadVertices[0], quadVerticesCount, false);
    gameState.testSprite.texCoordsBuffer = gapiCreateVec2fBuffer(&quadTexCoords[0], quadTexCoordsCount, false);
    gameState.testSprite.vao = gapiCreateVAO();

    gapiBindVAO(gameState.testSprite.vao);
    gapiCreateVector2fVAO(&gameState.testSprite.verticesBuffer, 0);
    gapiCreateVector2fVAO(&gameState.testSprite.texCoordsBuffer, 1);
}

static void initShaders() {
    const Result<AssetData> fragmentShaderResult = platformLoadAssetData(
        &gameState.memory.assetsBuffer,
        AssetType::shader,
        "fragment_texture.glsl"
    );

    const Result<AssetData> vertexShaderResult = platformLoadAssetData(
        &gameState.memory.assetsBuffer,
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

    Shader shaders[2];
    shaders[0] = vertexShader;
    shaders[1] = fragmentShader;

    const auto res = gapiCreateShaderProgram(
        "test_shader_program",
        StaticArray<Shader>{
            .size = 2,
            .items = shaders
        }
    );

    gameState.spriteShader = resultUnwrap(res);

    auto locRes = gapiGetShaderUniformLocation(gameState.spriteShader, "MVP");
    gameState.spriteShaderLocationMVP = resultUnwrap(locRes);

    locRes = gapiGetShaderUniformLocation(gameState.spriteShader, "utexture");
    gameState.spriteShaderLocationTexture = resultUnwrap(locRes);
}

static void initTexture() {
    const Result<AssetData> testTextureResult = platformLoadAssetData(
        &gameState.memory.assetsBuffer,
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
    renderSprite();
}

static void renderSprite() {
    gapiBindShaderProgram(gameState.spriteShader);

    gapiSetShaderProgramUniformMat4f(
        gameState.spriteShader,
        gameState.spriteShaderLocationMVP,
        gameState.testSpriteMVPMatrix
    );

    gapiSetShaderProgramUniformTexture2D(
        gameState.spriteShader,
        gameState.spriteShaderLocationTexture,
        gameState.spriteTexture,
        0
    );

    gapiBindVAO(gameState.testSprite.vao);
    gapiBindIndices(gameState.testSprite.indicesBuffer);
    gapiRenderIndexedGeometry(4, RenderMode::triangleStrip);
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
    gameState.testSpriteTransforms.rotation += (0.25f/1000.f) * deltaTime;

    gameState.testSpriteModelMatrix = gapiCreate2DModelMatrix(gameState.testSpriteTransforms);
    gameState.testSpriteMVPMatrix = gameState.cameraMatrices.mvpMatrix * gameState.testSpriteModelMatrix;
}
