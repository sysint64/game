#include "gapi.hpp"
#include "gapi_opengl.hpp"
#include "errors.hpp"

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

void gapiBindVAO(GeometryVAO* vao) {
    glBindVertexArray(vao->id);
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

void gapiBindIndices(GeometryBuffer* indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->id);
}

void gapiRenderIndexedGeometry(uint indicesLength, RenderMode renderMode) {
    switch (renderMode) {
        case RenderMode::triangles:
            glDrawElements(GL_TRIANGLES, indicesLength, GL_UNSIGNED_INT, nullptr);
            break;

        default:
            // TODO: log
            puts("unknown render mode");
    }
}
