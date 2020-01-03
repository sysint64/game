#pragma once

struct GApi;

struct GApiContext;

#ifdef GAPI_OPENGL
#include "gapi_opengl.hpp"

    #ifdef PLATFORM_SDL2
    #include "gapi_opengl_sdl2.hpp"
    #endif

    #ifdef PLATFORM_GLFW
    #include "gapi_opengl_glfw.hpp"
    #endif

#endif

#ifdef GAPI_VULKAN
#include "gapi_vulkan.hpp"

    #ifdef PLATFORM_SDL2
    #include "gapi_vulkan_sdl2.hpp"
    #endif

    #ifdef PLATFORM_GLFW
    #include "gapi_vulkan_glfw.hpp"
    #endif

#endif

#include "game_types.hpp"
#include "platform.hpp"
#include "assets.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct CameraMatrices {
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 mvpMatrix;
};

struct OthroCameraTransforms {
    glm::vec2 viewportSize;
    glm::vec2 position;
    float zoom;
};

enum class RenderMode {
    triangles,
    triangleStrip,
};

enum class ShaderType {
    vertex,
    fragment,
    geometry
};

struct GeometryBuffer;

struct GeometryVAO;

struct ShaderProgram;

struct Shader;

struct Texture2D;

struct Texture2DParameters {
    bool wrapS;
    bool wrapT;
    bool minFilter;
    bool magFilter;
};

struct Transforms2D {
    glm::vec2 position;
    glm::vec2 scaling;
    float rotation { 0.f };
};

// Camera

CameraMatrices gapiCreateOrthoCameraMatrices(const OthroCameraTransforms transforms);

// Transforms

glm::mat4 gapiCreate2DModelMatrix(Transforms2D transforms);

// Geometry

GeometryBuffer gapiCreateU32Buffer(const u32* data, size_t count, bool isDynamic = true);

GeometryBuffer gapiCreateF32Buffer(const f32* data, size_t count, bool isDynamic = true);

GeometryBuffer gapiCreateVec2fBuffer(const glm::vec2* data, size_t count, bool isDynamic = true);

GeometryBuffer gapiCreateVec3fBuffer(const glm::vec3* data, size_t count, bool isDynamic = true);

GeometryBuffer gapiReallocateU32Buffer(const u32* data, size_t count, bool isDynamic = true);

GeometryBuffer gapiReallocateVec2fBuffer(const glm::vec2* data, size_t count, bool isDynamic = true);

GeometryBuffer gapiReallocateVec3fBuffer(const glm::vec3* data, size_t count, bool isDynamic = true);

void gapiDeleteBuffer(GeometryBuffer* buffer);

GeometryVAO gapiCreateVAO();

void gapiBindVAO(GeometryVAO vao);

void gapiCreateVector2fVAO(GeometryBuffer* buffer, u32 location);

void gapiCreateVector3fVAO(GeometryBuffer* buffer, u32 location);

void gapiBindIndices(GeometryBuffer indices);

void gapiRenderIndexedGeometry(uint indicesLength, RenderMode renderMode = RenderMode::triangles);

// Shaders

Result<Shader> gapiCreateShader(const char* name, ShaderType shaderType, AssetData data);

Result<ShaderProgram> gapiCreateShaderProgram(const char* name, StaticArray<Shader> shaders);

void gapiBindShaderProgram(ShaderProgram program);

void gapiUnbindShaderProgram();

Result<u32> gapiGetShaderUniformLocation(ShaderProgram program, const char* location);

void gapiDeleteShader(Shader shader);

void gapiDeleteShaderProgram(ShaderProgram program);

// Shader Uniforms

void gapiSetShaderProgramUniformFloat(ShaderProgram program, u32 location, float val);

void gapiSetShaderProgramUniformInt(ShaderProgram program, u32 location, i32 val);

void gapiSetShaderProgramUniformUInt(ShaderProgram program, u32 location, u32 val);

void gapiSetShaderProgramUniformTexture2D(ShaderProgram program, u32 location, Texture2D val);

void gapiSetShaderProgramUniformVec2f(ShaderProgram program, u32 location, glm::vec2 val);

void gapiSetShaderProgramUniformVec3f(ShaderProgram program, u32 location, glm::vec3 val);

void gapiSetShaderProgramUniformVec4f(ShaderProgram program, u32 location, glm::vec4 val);

void gapiSetShaderProgramUniformMat4f(ShaderProgram program, u32 location, glm::mat4 matrix);

// Texture

Texture2D gapiCreateTexture2D(AssetData data, Texture2DParameters params);

//

void gapiClear(float r, float g, float b);

Result<GApiContext> gapiCreateContext(Platform platform, Window window);

Result<GApi> gapiInit();

void gapiSwapWindow(Platform platform, Window window);

void gapiShutdown(GApiContext context);
