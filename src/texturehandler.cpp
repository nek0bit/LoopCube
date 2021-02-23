#include "texturehandler.hpp"

TextureHandler::TextureHandler(SDL_Renderer* renderer)
{
    for (size_t i = 0; i < constants::textureInfo.size(); ++i) {
        textures.push_back(
            std::make_pair(i,
                           std::make_shared<Texture>(renderer,
                               constants::path + constants::textureInfo[i].textureFilename,
                               constants::textureInfo[i].w,
                               constants::textureInfo[i].h)
                ));	
    }
}

TextureHandler::~TextureHandler()
{}

Texture* TextureHandler::getTexture(size_t id)
{
    return textures[id].second.get();
}
