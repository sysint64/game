#include "memory.hpp"
#include "platform.hpp"

Result<RegionMemoryBuffer> createRegionMemoryBuffer(u64 size) {
    u8* base = platformAlloc(size);

    if (base) {
        auto buffer = RegionMemoryBuffer {
            .base = base,
            .size = size,
            .offset = 0,
        };
        return resultCreateSuccess<RegionMemoryBuffer>(buffer);
    }
    else {
        return resultCreateGeneralError<RegionMemoryBuffer>(
            ErrorCode::ALLOCATION_ERROR,
            "platformAlloc has failed"
        );
    }
}

Result<StackMemoryBuffer> createStackMemoryBuffer(RegionMemoryBuffer* root, u64 size) {
    StackMemoryBuffer buffer;

    if (root->offset + size > root->size) {
        return resultCreateGeneralError<StackMemoryBuffer>(
            ErrorCode::ALLOCATION_ERROR,
            "Out of memory"
        );
    }

    root->offset += size;
    return resultCreateSuccess(buffer);
}

void regionMemoryBufferAddRegion(RegionMemoryBuffer* where, RegionMemoryBuffer* buffer, u64 size) {
    assert(where->offset + size <= where->size);

    buffer->base = where->base + where->offset;
    buffer->size = size;
    buffer->offset = 0;

    where->offset += size;
}

Result<u8*> regionMemoryBufferAlloc(RegionMemoryBuffer* buffer, u64 size) {
    if (buffer->offset + size > buffer->size) {
        return resultCreateGeneralError<u8*>(
            ErrorCode::ALLOCATION_ERROR,
            "Out of memory"
        );
    }

    u8* result = buffer->base + buffer->offset;
    buffer->offset += size;

    return resultCreateSuccess(result);
}

void regionMemoryBufferFree(RegionMemoryBuffer* buffer) {
    buffer->offset = 0;
}
