#include "texturehandler.hpp"

TextureHandler::TextureHandler()
{
    for (size_t i = 0; i < constants::textureInfo.size(); ++i) {
        textures.push_back(
            std::make_pair(i, std::make_shared<Texture>(constants::path + constants::textureInfo[i].textureFilename)));	
    }
}

TextureHandler::~TextureHandler()
{}

Texture* TextureHandler::getTexture(size_t id)
{
    return textures[id].second.get();
}
