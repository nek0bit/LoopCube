#ifdef GRAPHIC_BACKEND_SFML

#ifndef GRAPHICSWRAPPER_SFML_HPP
#define GRAPHICSWRAPPER_SFML_HPP
#include <functional>
#include <string>

#include "backendincludes.hpp"

#include "config.hpp"
#include "texture.hpp"
#include "texture_sfml.hpp"
#include "texturehandler.hpp"

class GraphicsWrapper_SFML: public GraphicsWrapper {
public:
	GraphicsWrapper_SFML(Config config);
	~GraphicsWrapper_SFML();

	void init_screen() override;
	void clear_screen() override;
	void update_screen() override;
	void render(Rect&, Rect&, int) override;
	void render_rect(Rect&, Color) override;
	void fetch_screen_size();
	sf::RenderWindow* get_screen();
private:
	sf::RenderWindow* screen;
	TextureHandler<Texture_SFML*, std::function<Texture_SFML*(std::string)>>* textures;
	
	Config config;
};

#endif
#endif
