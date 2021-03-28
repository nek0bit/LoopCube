#include "texture.hpp"

Texture::Texture(const std::string filename)
{
    SDL_Surface* image = IMG_Load(filename.c_str());
    
    if (generateTextureFromSurface(image) == 1)
    {
        std::cerr << "Failed to load image at " << filename << std::endl;
    }

    // I think we're done here...
    SDL_FreeSurface(image);
}

Texture::Texture(SDL_Surface* texFromSurface)
{
    if (texFromSurface == nullptr) throw std::runtime_error("Recieved nullptr as argument!");
    if (generateTextureFromSurface(texFromSurface) == 1)
    {
        throw std::runtime_error("Got nullptr from texture!");
    }
}

int Texture::generateTextureFromSurface(SDL_Surface* surface)
{
    constexpr int SUCCESS = 0;
    constexpr int ERROR_NULLPTR = 1;
    // Generate and bind texture so we can set it up
    glGenTextures(1, &texture);
    bind();

    // Texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    if (surface != nullptr)
    {
        const int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
        
        // Load surface into texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
        return SUCCESS;
    }
    return ERROR_NULLPTR;
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
