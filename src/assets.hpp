#pragma once

#include "game_types.hpp"

enum AssetType {
    texture,
    sfx,
    music,
    shader,
    font,
};

struct AssetData {
    size_t size;
    void* data;
};

enum TextureFormat {
    rgb,
    rgba,
};

struct TextureHeader {
    i32 width;
    i32 height;
    TextureFormat format;
};
