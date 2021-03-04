#pragma once

struct TextureInfo
{
    constexpr TextureInfo(const char* textureFilename, const unsigned id,
                          const int sizeX = 0, const int sizeY = 0,
                          const int tilemapX = 0, const int tilemapY = 0)
        : textureFilename{textureFilename},
          id{id},
          sizeX{sizeX},
          sizeY{sizeY},
          tilemapX{tilemapX},
          tilemapY{tilemapY} {}
    const char* textureFilename;
    const unsigned id;
    const int sizeX, sizeY;
    const int tilemapX, tilemapY;
};
