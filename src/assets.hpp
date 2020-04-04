#pragma once

#include "game_types.hpp"

enum AssetType {
    texture,
    sfx,
    music,
    shader,
    font,
    level,
};

struct AssetData {
    size_t size;
    u8* data;
};

enum TextureFormat {
    rgb,
    rgba,
};

struct TextureHeader {
    u32 width;
    u32 height;
    TextureFormat format;
};
