#pragma once

#include "game_types.hpp"
#include "components.hpp"

struct Storage {
    StaticArray<RenderCommandComponent> renderCommands;
    SparseArray<glm::mat4> mvpMatrices;
};
