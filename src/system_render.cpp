#include "system_render.hpp"
#include "gapi_geometries.hpp"

void RenderSystem::init(GameMemory* memory, Storage* storage) {
    this->memory = memory;
    this->storage = storage;
    this->initGeometry();
    this->initShaders();
}

void RenderSystem::onRender() {
    for (int i = 0; i < storage->renderCommands.size; ++i) {
        const auto command = storage->renderCommands.items[i];
        applyCommand(command);
    }
}

void RenderSystem::applyCommand(RenderCommandComponent component) {
    switch (component.command) {
        case RenderCommand::ColorPipeline:
            applyColorPipelineCommand();
            break;

        case RenderCommand::TexturePipeline:
            applyTexturePipelineCommand();
            break;

        case RenderCommand::SetMVPShaderUniform:
            applySetMVPShaderUniformCommand(component.params);
            break;

        case RenderCommand::SetColorShaderUniform:
            applySetColorShaderUniformCommand(component.params);
            break;

        case RenderCommand::SetTextureShaderUniform:
            applySetTextureShaderUniformCommand(component.params);
            break;

        case RenderCommand::Quads:
            applyQuadsCommand();
            break;

        case RenderCommand::Lines:
            applyLinesCommand();
            break;

        case RenderCommand::Dots:
            applyDotsCommand();
            break;
    }
}

void RenderSystem::applyColorPipelineCommand() {
    gapiBindShaderProgram(colorShader);
    currentShader = &colorShader;
}

void RenderSystem::applyTexturePipelineCommand() {
    gapiBindShaderProgram(textureShader);
    currentShader = &colorShader;
}

void RenderSystem::applySetMVPShaderUniformCommand(RenderCommandParams params) {
    u32 location;

    if (currentShader == &colorShader) {
        location = colorShaderLocationMVP;
    }
    else if (currentShader == &textureShader) {
        location = textureShaderLocationMVP;
    }

    gapiSetShaderProgramUniformMat4f(
        *currentShader,
        location,
        params.mvpMatrix
    );
}

void RenderSystem::applySetColorShaderUniformCommand(RenderCommandParams params) {
    gapiSetShaderProgramUniformVec4f(
        colorShader,
        colorShaderLocationColor,
        params.color
    );
}

void RenderSystem::applySetTextureShaderUniformCommand(RenderCommandParams params) {
    gapiSetShaderProgramUniformTexture2D(
        textureShader,
        textureShaderLocationTexture,
        params.texture,
        params.textureIndex
    );
}

void RenderSystem::applyQuadsCommand() {
    gapiBindVAO(quadVao);
    gapiBindIndices(quadIndicesBuffer);
    gapiRenderIndexedGeometry(4, RenderMode::triangleStrip);
}

void RenderSystem::applyLinesCommand() {
    // TODO: Not implemented yet
}

void RenderSystem::applyDotsCommand() {
    // TODO: Not implemented yet
}

void RenderSystem::initGeometry() {
    quadIndicesBuffer = gapiCreateU32Buffer(&quadIndices[0], quadIndicesCount, false);
    quadVerticesBuffer = gapiCreateVec2fBuffer(&centeredQuadVertices[0], quadVerticesCount, false);
    quadTexCoordsBuffer = gapiCreateVec2fBuffer(&quadTexCoords[0], quadTexCoordsCount, false);
    quadVao = gapiCreateVAO();

    gapiBindVAO(quadVao);
    gapiCreateVector2fVAO(&quadVerticesBuffer, 0);
    gapiCreateVector2fVAO(&quadTexCoordsBuffer, 1);
}

void RenderSystem::initShaders() {
    const Result<AssetData> fragmentShaderSourceResult = platformLoadAssetData(
        &memory->assetsBuffer,
        AssetType::shader,
        "fragment_texture.glsl"
    );

    const Result<AssetData> vertexShaderSourceResult = platformLoadAssetData(
        &memory->assetsBuffer,
        AssetType::shader,
        "vertex_transform.glsl"
    );

    const AssetData fragmentShaderData = resultUnwrap(fragmentShaderSourceResult);
    const AssetData vertexShaderData = resultUnwrap(vertexShaderSourceResult);

    const auto vertexShaderResult = gapiCreateShader(
        "vertex_transform",
        ShaderType::vertex,
        vertexShaderData
    );
    const auto fragmentShaderResult = gapiCreateShader(
        "fragment_texture",
        ShaderType::fragment,
        fragmentShaderData
    );

    const Shader vertexShader = resultUnwrap(vertexShaderResult);
    const Shader fragmentShader = resultUnwrap(fragmentShaderResult);

    Shader shaders[2];
    shaders[0] = vertexShader;
    shaders[1] = fragmentShader;

    const auto res = gapiCreateShaderProgram(
        "texture_shader_program",
        StaticArray<Shader>{
            .size = 2,
            .items = shaders
        }
    );

    textureShader = resultUnwrap(res);

    auto locRes = gapiGetShaderUniformLocation(textureShader, "MVP");
    textureShaderLocationMVP = resultUnwrap(locRes);

    locRes = gapiGetShaderUniformLocation(textureShader, "utexture");
    textureShaderLocationTexture = resultUnwrap(locRes);
}
