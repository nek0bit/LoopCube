#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

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
	
	void update(Camera& camera, Time& time) override;
	void render(SDL_Renderer* renderer, TextureHandler& textures) override;
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
    BGCoord bgCave;

    // Size
    BGSize bgShine;
    BGSize bgCloud;
    BGSize bgHills;
    BGSize bgCaveBlock;
    BGSize bgLight;

    // Sky
    SDL_Color skyColor;
};
