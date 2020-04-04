#pragma once

#include "gapi.hpp"
#include "memory.hpp"
#include "components.hpp"
#include "storage.hpp"

class RenderSystem {
public:
    void init(GameMemory* memory, Storage* storage);
    void onRender();

private:
    GameMemory* memory;
    Storage* storage;

    GeometryBuffer quadIndicesBuffer;
    GeometryBuffer quadVerticesBuffer;
    GeometryBuffer quadTexCoordsBuffer;
    GeometryVAO quadVao;

    ShaderProgram* currentShader;
    ShaderProgram colorShader;
    ShaderProgram textureShader;
    u32 textureShaderLocationMVP;
    u32 textureShaderLocationTexture;
    u32 colorShaderLocationMVP;
    u32 colorShaderLocationColor;

    void initGeometry();

    void initShaders();

    void applyCommand(RenderCommandComponent component);

    void applyColorPipelineCommand();

    void applyTexturePipelineCommand();

    void applySetMVPShaderUniformCommand(RenderCommandParams params);

    void applySetColorShaderUniformCommand(RenderCommandParams params);

    void applySetTextureShaderUniformCommand(RenderCommandParams params);

    void applyQuadsCommand();

    void applyLinesCommand();

    void applyDotsCommand();
};
