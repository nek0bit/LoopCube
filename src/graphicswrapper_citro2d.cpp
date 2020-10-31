#include "graphicswrapper_citro2d.hpp"
#ifdef GRAPHIC_BACKEND_CITRO2D

GraphicsWrapper_Citro2D::GraphicsWrapper_Citro2D(Config config) {
	this->config = config;
}

GraphicsWrapper_Citro2D::~GraphicsWrapper_Citro2D() {
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}

void GraphicsWrapper_Citro2D::init_screen() {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	screen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	std::cout << "Welcome to LoopCube!" << std::endl;
}
void GraphicsWrapper_Citro2D::clear_screen() {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(screen, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	C2D_SceneBegin(screen);
}

void GraphicsWrapper_Citro2D::update_screen() {
	C3D_FrameEnd(0);
}

void GraphicsWrapper_Citro2D::render(Rect& src, Rect& dest, int) {
	C2D_DrawRectangle(dest.x, dest.y, 0, dest.w, dest.h, 255, 0, 0, 255);
}

void GraphicsWrapper_Citro2D::render_rect(Rect& dest, Color color) {
	C2D_DrawRectangle(dest.x, dest.y, 0, dest.w, dest.h, color.r, color.g, color.b, color.a); 
}

void GraphicsWrapper_Citro2D::fetch_screen_size() {
    SCREEN_W = 320;
    SCREEN_H = 240;
}
#endif
