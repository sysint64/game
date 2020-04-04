#pragma once

#include <cassert>
#include "game_types.hpp"

struct RegionMemoryBuffer {
    u8* base;
    u64 size;
    size_t offset;
};

struct StackMemoryBuffer {
    u8* base;
    u64 size;
    size_t offset;
};

struct ArenaMemoryBuffer {
    u8* base;
    u8* free;
    u64 size;
    u64 chunkSize;
    size_t offset;
};

struct GameMemory {
    RegionMemoryBuffer eternityBuffer;
    RegionMemoryBuffer componentsBuffer;
    RegionMemoryBuffer frameBuffer;
    RegionMemoryBuffer assetsBuffer;
    RegionMemoryBuffer tmpBuffer;
};

Result<RegionMemoryBuffer> createRegionMemoryBuffer(u64 size);

Result<StackMemoryBuffer> createStackMemoryBuffer(RegionMemoryBuffer* root, u64 size);

Result<ArenaMemoryBuffer> createArenaMemoryBuffer(RegionMemoryBuffer* root, u64 size, u64 chunkSize);

void regionMemoryBufferAddRegion(RegionMemoryBuffer* where, RegionMemoryBuffer* buffer, u64 size);

Result<u8*> regionMemoryBufferAlloc(RegionMemoryBuffer* buffer, u64 size);

void regionMemoryBufferFree(RegionMemoryBuffer* buffer);
