#ifdef GRAPHIC_BACKEND_SFML
#include "texture_sfml.hpp"

Texture_SFML::Texture_SFML(std::string filename) : Texture<sf::Texture>{} {
	if (!texture.loadFromFile(filename)) {
		char msg[] = "Error loading file ";
		strcat(msg, filename.c_str());
		throw std::runtime_error(msg);
	} else {
		rect.setFillColor(sf::Color::White);
	    rect.setTexture(&texture);		
	}
}

Texture_SFML::~Texture_SFML() { free_texture(); }

sf::RectangleShape& Texture_SFML::get_rect() {
	return rect;
}

void Texture_SFML::free_texture() {}

#endif
