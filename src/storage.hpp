#pragma once

#include "game_types.hpp"
#include "components.hpp"

class Storage {
public:
    DynamicArray<RenderCommandComponent> renderCommands;

    void init(GameMemory* memory);

    void removeEntity(u64 entity);

    StaticArray<OthroCameraTransforms> getCameras();

    void putMvpMatrices(SparseArrayEntry<glm::mat4>* entries);

private:
    SparseArray<glm::mat4> mvpMatrices;
    SparseArray<OthroCameraTransforms> cameras;
};
