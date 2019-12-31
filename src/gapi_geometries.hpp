#pragma once

#include <glm/glm.hpp>
#include "game_types.hpp"

const size_t quadVerticesCount = 4;
const glm::vec2 centeredQuadVertices[quadVerticesCount] = {
    glm::vec2(-0.5f, -0.5f),
    glm::vec2( 0.5f, -0.5f),
    glm::vec2( 0.5f,  0.5f),
    glm::vec2(-0.5f,  0.5f),
};

const glm::vec2 quadVertices[quadVerticesCount] = {
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f),
};

const size_t quadIndicesCount = 4;
const u32 quadIndices[quadIndicesCount] = { 0, 3, 1, 2 };

const size_t quadTexCoordsCount = 4;
const glm::vec2 quadTexCoords[quadTexCoordsCount] = {
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
};
