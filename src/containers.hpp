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
struct SparseArrayNode {
    u64 key;
    T value;
    SparseArrayNode<T>* next;
    // SparseArrayNode<T>* prev;
};

template<typename T>
struct SparseArrayIter {
    SparseArrayNode<T>* head;

    void init(SparseArrayNode<T>* head) {
        this->head = head;
    }

    inline T* next() {
        auto current = head;

        if (current != nullptr) {
            head = head->next;
        }

        return current;
    }
};

template<typename T>
struct SparseArray {
    u8* base;
    SparseArrayNode<T>* first;
    SparseArrayNode<T>* head;
    SparseArrayNode<T>* free;

    void init(RegionMemoryBuffer* buffer, u64 count) {
        const u64 cunkSize = sizeof(SparseArrayNode<T>);
        Result<u8*> baseResult = regionMemoryBufferAlloc(buffer, count * cunkSize);
        base = resultUnwrap(baseResult);

        head = nullptr;
        free = (SparseArrayNode<T>*) base;
        SparseArrayNode<T>* current = free;
        // current->prev = nullptr;

        for (size_t i = 0; i < count - 1; ++i) {
            current->next = (SparseArrayNode<T>*) (base + (i + 1) * cunkSize);

            // if (i > 0) {
                // current->prev = (SparseArrayNode<T>*) (base + (i - 1) * cunkSize);
            // }

            current = current->next;
        }

        current->next = nullptr;
    }

    void removeByKey(i64 key) {
        auto node = findNodeByKey(key);

        if (node == nullptr) {
            return;
        }

        if (node == first) {
            first = node->next;
        }

        node->next = free;
        // free->prev = node;
        // node->prev = nullptr;
        free = node;
    }

    Result<bool> put(u64 key, T value) {
        if (free == nullptr) {
            return resultCreateGeneralError<bool>(
                ErrorCode::ALLOCATION_ERROR,
                "Out of memory"
            );
        }
        else {
            auto current = free;
            free = current->next;

            current->key = key;
            current->value = value;
            current->next = nullptr;

            if (head == nullptr) {
                head = current;
                first = head;
            }
            else {
                head->next = current;
                head = current;
            }

            return resultCreateSuccess(true);
        }

        return resultCreateSuccess(true);
    }

    inline SparseArrayIter<T> iter() {
        return SparseArrayIter<T>(first);
    }

    SparseArrayNode<T>* findNodeByKey(u64 key) {
        while (auto node = iter().next() != nullptr) {
            if (node.key == key) {
                return node;
            }
        }

        return nullptr;
    }
};

template<typename T>
struct SparseArrayEntry {
    u64 key;
    T* item;
};
