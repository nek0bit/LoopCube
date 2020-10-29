#ifndef GRAPHICSWRAPPER_INTERNAL_HPP
#define GRAPHICSWRAPPER_INTERNAL_HPP
#include <iostream>
#include <array>

#include "rect.hpp"
#include "color.hpp"

class GraphicsWrapper {
public:
	GraphicsWrapper();
	virtual ~GraphicsWrapper();

	virtual void init_screen();
	virtual void clear_screen();
	virtual void update_screen();
	virtual void render(Rect&, Rect&, int);
	virtual void render_rect(Rect&, Color);
	virtual void fetch_screen_size();
	virtual std::array<int, 2> screen_size();
protected:
	int SCREEN_W;
	int SCREEN_H;
};


#endif // GRAPHICSWRAPPER_INTERNAL_HPP
