#include "texture.hpp"

Texture::Texture(const std::string filename)
{
    SDL_Surface* image = IMG_Load(filename.c_str());
    
    glGenTextures(1, &texture);
    bind();

    // Texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    
    if (image != nullptr)
    {
        const int mode = image->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
        
        // Load image into texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);
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
    return {static_cast<float>((tileX * info.tileSizeW) / static_cast<float>(info.width)),
        static_cast<float>((tileY * info.tileSizeH) / static_cast<float>(info.height))};
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
