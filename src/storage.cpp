#include "storage.hpp"

void Storage::init(GameMemory* memory) {
    renderCommands.init(&memory->frameBuffer, 100);
    mvpMatrices.init(&memory->componentsBuffer, 100);
    cameras.init(&memory->componentsBuffer, 100);
}

void Storage::removeEntity(u64 entity) {
    mvpMatrices.removeByKey(entity);
    cameras.removeByKey(entity);
}
