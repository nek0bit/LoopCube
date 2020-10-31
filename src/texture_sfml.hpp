#ifdef GRAPHIC_BACKEND_SFML
#include "backendincludes.hpp"

#ifndef TEXTURE_SFML_HPP
#define TEXTURE_SFML_HPP
#include <string>
#include <cstring>

#include "texture.hpp"


class Texture_SFML: public Texture<sf::Texture> {
public:
	Texture_SFML(std::string filename);
	~Texture_SFML();

    //sf::Texture& get_texture();
	sf::RectangleShape& get_rect();
	void free_texture() override;
private:
	sf::Texture texture;
	sf::RectangleShape rect;
};

#endif
#endif
