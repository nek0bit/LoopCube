#include "texturehandler.hpp"

TextureHandler::TextureHandler(Texture* (*create_texture)(std::string)) {
	for (size_t i = 0; i < constants::texture_ids.size(); ++i) {
		textures.push_back(std::make_pair(i, (*create_texture)(constants::path + constants::texture_ids[i] + ".png")));	
	}
}

TextureHandler::~TextureHandler() {
	free_textures();
}

void TextureHandler::free_textures() {
	for (auto &i: textures) {
		i.second.free_texture();
		delete i.second;
	}
}

Texture* TextureHandler::get_texture(int id) {
	for (auto &i: textures) {
		if (id == i.first) {
			return i.second;
		}
	}
	return nullptr;
}
