#ifdef GRAPHIC_BACKEND_CITRO2D

#ifndef GRAPHICSWRAPPER_CITRO2D_HPP
#define GRAPHICSWRAPPER_CITRO2D_HPP
#include <iostream>

#include "backendincludes.hpp"

#include "texture.hpp"
//#include "texture_citro2d.hpp"
#include "graphicswrapper_internal.hpp"
#include "texturehandler.hpp"
#include "rect.hpp"
#include "color.hpp"

class GraphicsWrapper_Citro2D: public GraphicsWrapper {
public:
	GraphicsWrapper_Citro2D(Config config);
	~GraphicsWrapper_Citro2D();

	void init_screen() override;
	void clear_screen() override;
	void update_screen() override;
	void render(Rect&, Rect&, int) override;
	void render_rect(Rect&, Color) override;
	void fetch_screen_size();
private:
	Config config;
	C3D_RenderTarget* screen;
};


#endif // GRAPHICSWRAPPER_CITRO2D_HPP

#endif // GRAPHIC_BACKEND_CITRO2D
