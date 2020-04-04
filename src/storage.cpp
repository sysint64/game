#include "storage.hpp"

void Storage::init(GameMemory* memory) {
    renderCommands.init(&memory->frameBuffer, 100);
    mvpMatrices.init(&memory->componentsBuffer, 100);
    cameras.init(&memory->componentsBuffer, 100);
}
