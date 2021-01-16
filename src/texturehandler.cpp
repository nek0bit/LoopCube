#include "texturehandler.hpp"

TextureHandler::TextureHandler(SDL_Renderer* renderer)
{
    for (size_t i = 0; i < constants::textureIds.size(); ++i) {
        textures.push_back(std::make_pair(i, new Texture{renderer, constants::path + constants::textureIds[i] + ".png"}));	
    }
}

TextureHandler::~TextureHandler()
{
    freeTextures();
}

SDL_Texture* TextureHandler::getTexture(int id)
{
    for (auto &i: textures) {
        if (id == i.first) {
            return i.second->getTexture();
        }
    }
    return nullptr;
}

void TextureHandler::freeTextures()
{
    for (auto &i: textures) {
        i.second->freeTexture();
        delete i.second;
    }
}
