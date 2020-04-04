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

struct Arena {
    u8* data;
    Arena* next;
};

struct ArenaMemoryBuffer {
    Arena* head;
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

Result<Arena*> arenaMemoryBufferAlloc(ArenaMemoryBuffer* buffer);

void arenaMemoryBufferFree(ArenaMemoryBuffer* buffer, Arena* arena);
