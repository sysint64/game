#pragma once

#include <GL/glew.h>
#include "game_types.hpp"

struct GApi {
};

struct GeometryBuffer {
    GLuint id;
};

struct GeometryVAO {
    GLuint id;
};

struct Shader {
    GLuint id;
    const char* name;
};

struct ShaderProgram {
    GLuint id;
    const char* name;
};

struct Texture2D {
    GLuint id = 0;
    u32 width;
    u32 height;
};
