#ifdef GRAPHIC_BACKEND_SFML

#include "graphicswrapper_sfml.hpp"

GraphicsWrapper_SFML::GraphicsWrapper_SFML(Config config) : GraphicsWrapper{} {
	this->config = config;

	auto call = [&](std::string filename)->Texture_SFML* {
		return new Texture_SFML{filename};
	};

	textures = new TextureHandler<Texture_SFML*, std::function<Texture_SFML*(std::string)>>(call);
	
}

GraphicsWrapper_SFML::~GraphicsWrapper_SFML() {

}

void GraphicsWrapper_SFML::init_screen() {
    screen = new sf::RenderWindow{sf::VideoMode(800, 600), "SFML Window"};

	screen->setVerticalSyncEnabled(true);
}

void GraphicsWrapper_SFML::clear_screen() {
	screen->clear(sf::Color::Black);
}

void GraphicsWrapper_SFML::update_screen() {
    screen->display();
}

void GraphicsWrapper_SFML::render(Rect& src, Rect& dest, int texture_index) {
	sf::RectangleShape& tmp = textures->get_texture(texture_index)->get_rect();

	tmp.setPosition(dest.x, dest.y);
	tmp.setSize(sf::Vector2f(dest.w, dest.h));
	tmp.setTextureRect(sf::IntRect(src.x, src.y, src.w, src.h));
  
    screen->draw(tmp);
}

void GraphicsWrapper_SFML::render_rect(Rect& dest, Color color) {
	sf::RectangleShape rect(sf::Vector2f(dest.w, dest.h));
	rect.setPosition(dest.x, dest.y);
	rect.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	
	screen->draw(rect);
}

void GraphicsWrapper_SFML::fetch_screen_size() {
	sf::Vector2u size = screen->getSize();
    SCREEN_W = size.x;
    SCREEN_H = size.y;
}

#endif
