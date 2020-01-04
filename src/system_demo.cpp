#include "system_demo.hpp"
#include "gapi.hpp"
#include "gapi_geometries.hpp"

void DemoSystem::init(GameMemory* memory) {
    this->memory = memory;
    this->initSprite();
    this->initShaders();
    this->initTexture();
}

void DemoSystem::onRender() {
    gapiBindShaderProgram(spriteShader);

    gapiSetShaderProgramUniformMat4f(
        spriteShader,
        spriteShaderLocationMVP,
        testSpriteMVPMatrix
    );

    gapiSetShaderProgramUniformTexture2D(
        spriteShader,
        spriteShaderLocationTexture,
        testSpriteTexture,
        0
    );

    gapiBindVAO(quadVao);
    gapiBindIndices(quadIndicesBuffer);
    gapiRenderIndexedGeometry(4, RenderMode::triangleStrip);
}

void DemoSystem::onProgress(float deltaTime) {
    cameraMatrices = gapiCreateOrthoCameraMatrices(cameraTransform);

    testSpriteTransforms.position = glm::vec2(
        cameraTransform.viewportSize.x / 2.f,
        cameraTransform.viewportSize.y / 2.f
    );
    testSpriteTransforms.scaling = glm::vec2(430.0f, 600.0f);
    testSpriteTransforms.rotation += (0.25f/1000.f) * deltaTime;

    testSpriteModelMatrix = gapiCreate2DModelMatrix(testSpriteTransforms);
    testSpriteMVPMatrix = cameraMatrices.mvpMatrix * testSpriteModelMatrix;
}

void DemoSystem::initSprite() {
    quadIndicesBuffer = gapiCreateU32Buffer(&quadIndices[0], quadIndicesCount, false);
    quadVerticesBuffer = gapiCreateVec2fBuffer(&centeredQuadVertices[0], quadVerticesCount, false);
    quadTexCoordsBuffer = gapiCreateVec2fBuffer(&quadTexCoords[0], quadTexCoordsCount, false);
    quadVao = gapiCreateVAO();

    gapiBindVAO(quadVao);
    gapiCreateVector2fVAO(&quadVerticesBuffer, 0);
    gapiCreateVector2fVAO(&quadTexCoordsBuffer, 1);
}

void DemoSystem::initShaders() {
    const Result<AssetData> fragmentShaderResult = platformLoadAssetData(
        &memory->assetsBuffer,
        AssetType::shader,
        "fragment_texture.glsl"
    );

    const Result<AssetData> vertexShaderResult = platformLoadAssetData(
        &memory->assetsBuffer,
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

    spriteShader = resultUnwrap(res);

    auto locRes = gapiGetShaderUniformLocation(spriteShader, "MVP");
    spriteShaderLocationMVP = resultUnwrap(locRes);

    locRes = gapiGetShaderUniformLocation(spriteShader, "utexture");
    spriteShaderLocationTexture = resultUnwrap(locRes);
}

void DemoSystem::initTexture() {
    const Result<AssetData> testTextureResult = platformLoadAssetData(
        &memory->assetsBuffer,
        AssetType::texture,
        "test.jpg"
    );

    const AssetData testTexture = resultUnwrap(testTextureResult);
    const Texture2DParameters params = {
        .minFilter = true,
        .magFilter = true,
    };
    testSpriteTexture = gapiCreateTexture2D(testTexture, params);
}
