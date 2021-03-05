#include "texture.hpp"

Texture::Texture(const std::string filename)
{
    SDL_Surface* image = IMG_Load(filename.c_str());
    
    glGenTextures(1, &texture);
    bind();

    // Texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    if (image != nullptr)
    {
        const int mode = image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
        
        // Load image into texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }

    // We're done here
    SDL_FreeSurface(image);
}

// Move
Texture::Texture(Texture&& source)
{
    texture = source.texture;
    source.texture = 0;
}


texcoord_t Texture::getTilemapCoord(const texcoord_info info, const unsigned tileX, const unsigned tileY) noexcept
{
    // The base size (less than 1.0f);
    const float baseX = static_cast<float>(info.tileSizeW) / static_cast<float>(info.width);
    const float baseY = static_cast<float>(info.tileSizeH) / static_cast<float>(info.height);

    // End results
    const float resX = static_cast<float>((tileX+1) * info.tileSizeW) /
        static_cast<float>(info.width);
    const float resY = static_cast<float>((tileY+1) * info.tileSizeH) /
        static_cast<float>(info.height);
    
    return {resX - baseX < 0 ? 0 : resX - baseX,
        resX,
        resY - baseY < 0 ? 0 : resY - baseY,
        resY};
}

Texture::~Texture()
{
    freeTexture();
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::freeTexture()
{
    glDeleteTextures(1, &texture);
}
