#pragma once

#include <GL/glew.h>

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
};
