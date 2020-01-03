#include "gapi.hpp"
#include "gapi_opengl.hpp"
#include "errors.hpp"

static Result<GLint> checkShaderStatus(const Shader shader, const GLenum pname);
static Result<GLint> checkProgramStatus(const ShaderProgram program, const GLenum pname);

Result<GApi> gapiInit() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    return resultCreateSuccess(GApi());
}

void gapiClear(float r, float g, float b) {
    glClearColor(r, g, b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

static GeometryBuffer createGeometryBuffer(const void* data, size_t size, bool isDynamic) {
    GeometryBuffer buffer;

    glGenBuffers(1, &buffer.id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.id);

    if (isDynamic) {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    } else {
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    return buffer;
}

void gapiDeleteBuffer(GeometryBuffer* buffer) {
    glDeleteBuffers(1, &buffer->id);
}

GeometryBuffer gapiCreateU32Buffer(const u32* data, size_t count, bool isDynamic) {
    return createGeometryBuffer(data, sizeof(u32) * count, isDynamic);
}

GeometryBuffer gapiCreateF32Buffer(const f32* data, size_t count, bool isDynamic) {
    return createGeometryBuffer(data, sizeof(f32) * count, isDynamic);
}

GeometryBuffer gapiCreateVec2fBuffer(const glm::vec2* data, size_t count, bool isDynamic) {
    return createGeometryBuffer(data, sizeof(f32) * 2 * count, isDynamic);
}

GeometryBuffer gapiCreateVec3fBuffer(const glm::vec3* data, size_t count, bool isDynamic) {
    return createGeometryBuffer(data, sizeof(f32) * 3 * count, isDynamic);

}
GeometryVAO gapiCreateVAO() {
    GeometryVAO vao;
    glGenVertexArrays(1, &vao.id);
    return vao;
}

void gapiBindVAO(GeometryVAO vao) {
    glBindVertexArray(vao.id);
}

void gapiCreateVector2fVAO(GeometryBuffer* buffer, u32 location) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void gapiCreateVector3fVAO(GeometryBuffer* buffer, u32 location) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void gapiBindIndices(GeometryBuffer indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.id);
}

void gapiRenderIndexedGeometry(uint indicesLength, RenderMode renderMode) {
    switch (renderMode) {
        case RenderMode::triangles:
            glDrawElements(GL_TRIANGLES, indicesLength, GL_UNSIGNED_INT, nullptr);
            break;

        case RenderMode::triangleStrip:
            glDrawElements(GL_TRIANGLE_STRIP, indicesLength, GL_UNSIGNED_INT, nullptr);
            break;

        default:
            // TODO: log
            puts("unknown render mode");
            exit(1);
    }
}

Texture2D gapiCreateTexture2D(const AssetData data, const Texture2DParameters params) {
    Texture2D texture;
    TextureHeader textureHeader = *((TextureHeader*) data.data);
    u8* textureData = data.data + sizeof(TextureHeader);

    texture.width = textureHeader.width;
    texture.height = textureHeader.height;

    GLenum format;

    switch (textureHeader.format) {
        case TextureFormat::rgb:
            format = GL_RGB;
            break;

        case TextureFormat::rgba:
            format = GL_RGBA;
            break;
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // TODO: Not sure if I should use it
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        /* target */ GL_TEXTURE_2D,
        /* level */ 0,
        /* internalformat */ format,
        /* width */ texture.width,
        /* height */ texture.height,
        /* border */ 0,
        /* format */ format,
        /* type */ GL_UNSIGNED_BYTE,
        /* data */ textureData
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return texture;
}

void gapiDeleteTexture2D(Texture2D texture) {
    glDeleteTextures(1, &texture.id);
}

void gapiBindTexture2D(Texture2D texture) {
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

void gapiUnbindTexture2D() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Shaders

Result<Shader> gapiCreateShader(const char* name, const ShaderType shaderType, AssetData data) {
    Shader shader;
    shader.name = name;
    GLenum glShaderType;

    switch (shaderType) {
        case ShaderType::fragment:
            glShaderType = GL_FRAGMENT_SHADER;
            break;

        case ShaderType::vertex:
            glShaderType = GL_VERTEX_SHADER;
            break;

        default:
            return resultCreateGeneralError<Shader>(
                ErrorCode::GAPI_CREATE_SHADER,
                "Unknown shader type %d", shaderType
            );
    }

    shader.id = glCreateShader(glShaderType);

    char* source = (char*) data.data;
    glShaderSource(shader.id, 1, &source, nullptr);
    glCompileShader(shader.id);
    const auto statusReault = checkShaderStatus(shader, GL_COMPILE_STATUS);

    if (resultHasError(statusReault)) {
        return switchError<Shader>(statusReault);
    }

    return resultCreateSuccess(shader);
}

static Result<GLint> checkShaderStatus(const Shader shader, const GLenum pname) {
    GLint status, length;
    GLchar message[1024] { 0 };

    glGetShaderiv(shader.id, pname, &status);

    if (status != GL_TRUE) {
        glGetShaderInfoLog(shader.id, 1024, &length, &message[0]);
        const char* reason;

        switch (pname) {
            case GL_COMPILE_STATUS:
                reason = "Failed when compiling shader";
                break;

            default:
                reason = "Failed";
                break;
        }

        return resultCreateGeneralError<GLint>(
            ErrorCode::GAPI_SHADER_STATUS,
            "%s. shader name: '%s', status: %d, message: '%s'",
            reason, shader.name, status, message
        );
    }

    return resultCreateSuccess(status);
}

Result<ShaderProgram> gapiCreateShaderProgram(const char* name, StaticArray<Shader> shaders) {
    ShaderProgram program;

    program.id = glCreateProgram();
    program.name = name;

    for (size_t i = 0; i < shaders.size; i += 1) {
        glAttachShader(program.id, shaders.items[i].id);
        puts(shaders.items[i].name);
    }

    glLinkProgram(program.id);
    const auto statusReault = checkProgramStatus(program, GL_LINK_STATUS);

    if (resultHasError(statusReault)) {
        return switchError<ShaderProgram>(statusReault);
    }

    return resultCreateSuccess(program);
}

static Result<GLint> checkProgramStatus(const ShaderProgram program, const GLenum pname) {
    GLint status, length;
    GLchar message[1024] { 0 };

    glGetProgramiv(program.id, pname, &status);

    if (status != GL_TRUE) {
        glGetProgramInfoLog(program.id, 1024, &length, &message[0]);
        const char* reason;

        switch (pname) {
            case GL_VALIDATE_STATUS:
                reason = "Failed when validation shader program";
                break;

            case GL_LINK_STATUS:
                reason = "Failed on linking program";
                break;

            default:
                reason = "Failed";
        }

        return resultCreateGeneralError<GLint>(
            ErrorCode::GAPI_SHADER_PROGRAM_STATUS,
            "%s. program name: '%s', status: %d, message: '%s'",
            reason, program.name, status, message
        );
    }

    return resultCreateSuccess(status);
}

void gapiBindShaderProgram(const ShaderProgram program) {

#ifdef VALIDATE
    glValidateProgram(program.id);
    const auto statusReault = checkProgramStatus(program, GL_VALIDATE_STATUS);
    resultUnwrap(statusReault);
#endif

    glUseProgram(program.id);
}

void gapiUnbindShaderProgram() {
    glUseProgram(0);
}

Result<u32> gapiGetShaderUniformLocation(const ShaderProgram program, const char* location) {
    const GLint loc = glGetUniformLocation(program.id, location);

    if (loc == -1) {
        return resultCreateGeneralError<u32>(
            ErrorCode::GAPI_SHADER_UNIFORM_LOCATION,
            "Failed to get uniform location. name '%s', location: %s",
            program.name, location
        );
    }
    else {
        return resultCreateSuccess((u32) loc);
    }
}

void gapiDeleteShader(const Shader shader) {
    glDeleteShader(shader.id);
}

void gapiDeleteShaderProgram(const ShaderProgram program) {
    glDeleteProgram(program.id);
}

// Shader Uniforms

void gapiSetShaderProgramUniformFloat(ShaderProgram program, u32 location, float val) {
    glUniform1f(location, val);
}

void gapiSetShaderProgramUniformInt(ShaderProgram program, u32 location, i32 val) {
    glUniform1i(location, val);
}

void gapiSetShaderProgramUniformUInt(ShaderProgram program, u32 location, u32 val) {
    glUniform1ui(location, val);
}

void gapiSetShaderProgramUniformTexture2D(ShaderProgram program, u32 location, Texture2D val, i32 index) {
    glActiveTexture(GL_TEXTURE0 + index);
    gapiBindTexture2D(val);
    glUniform1i(location, index);
}

void gapiSetShaderProgramUniformVec2f(ShaderProgram program, u32 location, glm::vec2 val) {
    glUniform2fv(location, 1, glm::value_ptr(val));
}

void gapiSetShaderProgramUniformVec3f(ShaderProgram program, u32 location, glm::vec3 val) {
    glUniform3fv(location, 1, glm::value_ptr(val));
}

void gapiSetShaderProgramUniformVec4f(ShaderProgram program, u32 location, glm::vec4 val) {
    glUniform4fv(location, 1, glm::value_ptr(val));
}

void gapiSetShaderProgramUniformMat4f(ShaderProgram program, u32 location, glm::mat4 matrix) {
    glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(matrix));
}
