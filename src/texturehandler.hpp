#ifndef TEXTUREHANDLER_HPP
#define TEXTUREHANDLER_HPP
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>

//#include "backendincludes.hpp"

#include "texture.hpp"
#include "constants.hpp"

template<class T, class U>
class TextureHandler {
public:
	// Pass a function (lambda preferable) that returns the created texture
	TextureHandler(U func) {
		for (size_t i = 0; i < constants::texture_ids.size(); ++i) {
			textures.push_back(std::make_pair(i, func(constants::path + constants::texture_ids[i] + ".png")));	
		}
	};
	~TextureHandler() { free_textures(); };

    T get_texture(int id) {
		for (auto &i: textures) {
			if (id == i.first) {
				return i.second;
			}
		}
		return nullptr;
	};
	void free_textures() {
		for (auto &i: textures) {
			i.second.free_texture();
			delete i.second;
		}
	};
private:
	std::vector<std::pair<int, T>> textures;
};


#endif // TEXTUREHANDLER_HPP
