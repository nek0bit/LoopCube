#include "texturehandler.hpp"

TextureHandler::TextureHandler()
{
}

TextureHandler::~TextureHandler()
{}

void TextureHandler::init()
{
    for (size_t i = 0; i < constants::textureInfo.size(); ++i) {
        textures.emplace_back(std::string(constants::path) + constants::textureInfo[i].textureFilename);
    }
}
