#ifndef GRAPHICSWRAPPER_INTERNAL_HPP
#define GRAPHICSWRAPPER_INTERNAL_HPP
#include <iostream>
#include <array>

#include "config.hpp"
#include "rect.hpp"
#include "color.hpp"

class GraphicsWrapper {
public:
	GraphicsWrapper();
	virtual ~GraphicsWrapper();

	virtual void init_screen();
	virtual void clear_screen();
	virtual void update_screen();
	virtual void render(Rect, Rect, int);
	virtual void render_rect(Rect, Color);
	virtual std::array<2, int> screen_size();
};


#endif // GRAPHICSWRAPPER_INTERNAL_HPP
