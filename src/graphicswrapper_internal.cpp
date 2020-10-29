#include "graphicswrapper_internal.hpp"

GraphicsWrapper::GraphicsWrapper() : SCREEN_W{0}, SCREEN_H{0} {}

GraphicsWrapper::~GraphicsWrapper() {}

void GraphicsWrapper::init_screen() {}
void GraphicsWrapper::clear_screen() {}
void GraphicsWrapper::update_screen() {}
void GraphicsWrapper::render(Rect& src, Rect& dest, int texture_index) {}
void GraphicsWrapper::render_rect(Rect& dest, Color color) {}
void GraphicsWrapper::fetch_screen_size() {};
std::array<int, 2> GraphicsWrapper::screen_size() { return std::array<int, 2>{SCREEN_W, SCREEN_H}; }
