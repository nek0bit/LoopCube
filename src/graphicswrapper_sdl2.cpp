#include "graphicswrapper_sdl2.hpp"

GraphicsWrapper_SDL2::GraphicsWrapper_SDL2(Config config) : GraphicsWrapper{} {
	this->config = config;
}

GraphicsWrapper_SDL2::~GraphicsWrapper_SDL2() {

}

void GraphicsWrapper_SDL2::init_screen() {
	int flags = 0;
	flags = flags | SDL_WINDOW_FULLSCREEN;
	flags = flags | SDL_WINDOW_RESIZABLE;
	flags = flags | SDL_RENDERER_ACCELERATED;
	flags = flags | SDL_RENDERER_PRESENTVSYNC;

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	} else {
		throw std::runtime_error(strcpy("SDL_Init failed: ", SDL_GetError()));
	}
}

void GraphicsWrapper_SDL2::clear_screen() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void GraphicsWrapper_SDL2::update_screen() {
	// Incomplete implementation
}

void GraphicsWrapper_SDL2::render(Rect src, Rect dest, int texture_index) {
	// Incomplete implementation
}

void GraphicsWrapper_SDL2::render_rect(Rect dest, Color color) {
	SDL_Rect dest_sdl{dest.x, dest.y, dest.w, dest.h};
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dest_sdl);
}

std::array<2, int> GraphicsWrapper_SDL2::screen_size() {
	int WINDOW_W;
	int WINDOW_H;
	SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
	return std::array<2, int>{WINDOW_W, WINDOW_H};
}
