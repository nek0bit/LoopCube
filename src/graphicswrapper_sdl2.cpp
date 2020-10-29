#include "graphicswrapper_sdl2.hpp"

GraphicsWrapper_SDL2::GraphicsWrapper_SDL2(Config config) : GraphicsWrapper{} {
	this->config = config;

}

GraphicsWrapper_SDL2::~GraphicsWrapper_SDL2() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void GraphicsWrapper_SDL2::init_screen() {
	// TODO use config for this
	int flags = 0;
	//flags = flags | SDL_WINDOW_FULLSCREEN;
	flags = flags | SDL_WINDOW_RESIZABLE;
	flags = flags | SDL_RENDERER_ACCELERATED;
	flags = flags | SDL_RENDERER_PRESENTVSYNC;

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	} else {
		char error[] = "SDL_Init failed: ";
		strcat(error, SDL_GetError());
		throw std::runtime_error(error);
	}

	flags = IMG_INIT_PNG;
	if ((IMG_Init(flags)&flags) != flags) {
		char error[] = "IMG_Init failed: ";
		strcat(error, IMG_GetError());
		throw std::runtime_error(error);
	}

	
    auto call = [&](std::string filename)->Texture_SDL2* {
		return new Texture_SDL2{renderer, filename};
	};
	
	textures = new TextureHandler<Texture_SDL2*, std::function<Texture_SDL2*(std::string)>>(call);
}

void GraphicsWrapper_SDL2::clear_screen() {
	// Reset render color to black for clearing
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void GraphicsWrapper_SDL2::update_screen() {
	SDL_RenderPresent(renderer);
}

void GraphicsWrapper_SDL2::render(Rect& src, Rect& dest, int texture_index) {
	SDL_Rect src_sdl{src.x, src.y, src.w, src.h};
	SDL_Rect dest_sdl{dest.x, dest.y, dest.w, dest.h};
	SDL_RenderCopy(renderer, textures->get_texture(texture_index)->get_texture(), &src_sdl, &dest_sdl);
}

void GraphicsWrapper_SDL2::render_rect(Rect& dest, Color color) {
	SDL_Rect dest_sdl{dest.x, dest.y, dest.w, dest.h};
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dest_sdl);
}

void GraphicsWrapper_SDL2::fetch_screen_size() {
	SDL_GetWindowSize(window, &SCREEN_W, &SCREEN_H);
}
