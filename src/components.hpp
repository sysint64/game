#pragma once

enum RenderCommand {
    ColorPipeline,
    TexturePipeline,
    SetMVPShaderUniform,
    SetColorShaderUniform,
    SetTextureShaderUniform,
    Quads,
    Lines,
    Dots,
};

struct RenderCommandParams {
    glm::mat4 mvpMatrix;
    glm::vec4 color;
    Texture2D texture;
    i32 textureIndex;
};

struct RenderCommandComponent {
    RenderCommand command;
    RenderCommandParams params;
};
