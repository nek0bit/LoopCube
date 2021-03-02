#pragma once

struct TextureInfo
{
    constexpr TextureInfo(const char* textureFilename, const unsigned id)
        : textureFilename{textureFilename},
          id{id} {}
    const char* textureFilename;
    const unsigned id;
};
