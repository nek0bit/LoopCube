#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <iostream>
#include <string>
#include <memory>

#include "backendincludes.hpp"

#include "uielement.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "text.hpp"
#include "aabb.hpp"

class Button: public UiElement {
public:
	Button() = default;
	Button(int id, TextureHandler& textures, int x, int y, int width, int height = 32);
	~Button();

	void set_x(int x);
	void set_y(int y);
	void update(EventWrapper*& events);
	void render(SDL_Renderer* renderer);

	// Text
	void set_text(SDL_Renderer* renderer, std::string text);
	std::string get_text();
	bool get_pressed();
	int get_id();
private:
	TextureHandler& textures;
	
	std::string text;
	unsigned int id;
	int x;
	int y;
	int width;
	int height;

	// Mouse state
	bool hovered;
	bool being_clicked;
	bool clicked;

	SDL_Rect src, dest;
	//std::shared_ptr<Text> button_text;

};

#endif // BUTTON_HPP
