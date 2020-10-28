#include "graphicswrapper_internal.hpp"

GraphicsWrapper::GraphicsWrapper() {}

GraphicsWrapper::~GraphicsWrapper() {}

void GraphicsWrapper::init_screen() {}
void GraphicsWrapper::clear_screen() {}
void GraphicsWrapper::update_screen() {}
void GraphicsWrapper::render(Rect src, Rect dest, int texture_index) {}
void GraphicsWrapper::render_rect(Rect dest, Color color) {}
std::array<2, int> GraphicsWrapper::screen_size() { return std::array<2, int>{0, 0}; }
