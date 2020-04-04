#pragma once

#include "game_types.hpp"
#include "memory.hpp"

template<typename T>
struct StaticArray {
    size_t size;
    T* base;

    inline T* getItems() {
        return (T*) base;
    }

    inline size_t getCount() {
        return size;
    }
};

template<typename T>
inline StaticArray<T> createStaticArray(u64 size, T* items) {
    return StaticArray<T> {
        .size = size,
        .base = items,
    };
}

template<typename T>
struct DynamicArray {
    void init(RegionMemoryBuffer* buffer, u64 count) {
        auto baseResult = regionMemoryBufferAlloc(buffer, count * sizeof(T));
        base = resultUnwrap(baseResult);
        count = 0;
        offset = 0;
    }

    void clear() {
        count = 0;
        offset = 0;
    }

    inline T* getItems() {
        return (T*) base;
    }

    inline size_t getCount() {
        return 0;
    }

private:
    size_t size;
    u8* base;
    u64 count;
    size_t offset;
};

template<typename T>
struct SparseArray {
    ArenaMemoryBuffer itemsMemory;
    ArenaMemoryBuffer keysMemory;

    void init(RegionMemoryBuffer* buffer, u64 count) {
        auto bufferResult = createArenaMemoryBuffer(buffer, count, sizeof(T));
        itemsMemory = resultUnwrap(bufferResult);

        bufferResult = createArenaMemoryBuffer(buffer, count, sizeof(u64));
        keysMemory = resultUnwrap(bufferResult);
    }

    void removeByKey(i64 key) {
    }

    void put(u64 key, T value) {

    }
};

template<typename T>
struct SparseArrayEntry {
    u64 key;
    T* item;
};
