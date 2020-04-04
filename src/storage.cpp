#include "storage.hpp"

void Storage::init(GameMemory* memory) {
    renderCommands.init(&memory->frameBuffer, megabytes(1));
    mvpMatrices.init(&memory->componentsBuffer, megabytes(1));
    cameras.init(&memory->componentsBuffer, megabytes(1));
}

void Storage::removeEntity(u64 entity) {
    mvpMatrices.removeByKey(entity);
}
