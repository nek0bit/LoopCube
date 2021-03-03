#pragma once

struct TextureInfo
{
    constexpr TextureInfo(const char* textureFilename, const unsigned id, const int tilemapX, const int tilemapY)
        : textureFilename{textureFilename},
          id{id},
          tilemapX{tilemapX},
          tilemapY{tilemapY} {}
    const char* textureFilename;
    const unsigned id;
    const int tilemapX;
    const int tilemapY;
};
