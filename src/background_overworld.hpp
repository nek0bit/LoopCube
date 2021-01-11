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
	void renderRepeating(SDL_Renderer* renderer, TextureHandler& textures, int texture, int offsetX, int offsetY, int width,
                         int height, int gap, int top, bool verticle = false, int srcW = -1, int srcH = -1);
	int winWidth;
	int winHeight;

	bool showCaveBackground;
    Transition bgCaveOpacity;

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
