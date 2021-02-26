#pragma once
#define _USE_MATH_DEFINES
#undef __STRICT_ANSI__
#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "graphics.hpp"
#include "generic.hpp"
#include "background.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "camera.hpp"
#include "time.hpp"
#include "transition.hpp"

struct BackgroundOverworld: public Background
{
	BackgroundOverworld();
	~BackgroundOverworld();
	
	void update(const Camera& camera, Time& time) override;
	void render(const Graphics& graphics, TextureHandler& textures) const override;
private:
	int winWidth;
	int winHeight;

    // Src, Dest
	SDL_Rect bgShineSrc, bgShineDest;
    SDL_Rect bgLightSrc, bgLightDest;
	SDL_Rect bgMoonSrc, bgMoonDest;

    // Coordinates
    BGCoord bgCloudOffset;
    BGCoord bgHillsOffset;

    // Size
    BGSize bgShine;
    BGSize bgCloud;
    BGSize bgHills;
    BGSize bgLight;

    // Sky
    SDL_Color skyColor;
};
