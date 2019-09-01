#pragma once

#include "game_types.hpp"
#include "platform.hpp"

struct MemoryBuffer {
    void* base;
    u64 size;
    umm used;
    MemoryBuffer* prev;
};

struct Memory {
    MemoryBuffer* currentBuffer;
};

struct TemporaryMemory {
    Memory* arena;
    MemoryBuffer* block;
    umm used;
};

inline void pushBufferToMemory(Memory* memory, MemoryBuffer* buffer) {
    buffer->prev = memory->currentBuffer;
    memory->currentBuffer = buffer;
}

inline Result<MemoryBuffer> allocMemoryBuffer(u64 size) {
    void* base = platformAlloc(size);

    if (base) {
        auto buffer = MemoryBuffer {
            .base = base,
            .size = size,
            .used = 0,
            .prev = nullptr,
        };
        return resultCreateSuccess<MemoryBuffer>(buffer);
    }
    else {
        return resultCreateError<MemoryBuffer>("allocate", "platformAlloc has failed");
    }
}
