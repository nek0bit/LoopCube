#include "background_overworld.hpp"

BackgroundOverworld::BackgroundOverworld()
	: Background{},
      winWidth{0},
      winHeight{0},
      showCaveBackground{false},
      bgCaveOpacity{0, 0.20},
      bgShineSrc{},
      bgShineDest{},
      bgLightSrc{},
      bgLightDest{},
      bgMoonSrc{},
      bgMoonDest{},
      bgCloudOffset{0, 0},
      bgHillsOffset{0, 0},
      bgCave{0, 0},
      bgShine{10, 150},
      bgCloud{450, 100},
      bgHills{450*2, 114*2},
      bgCaveBlock{60, 60},
      bgLight{100, 100},
      skyColor{0, 0, 0, 255}
{}

BackgroundOverworld::~BackgroundOverworld()
{}

void BackgroundOverworld::update(Camera& camera, Time& time)
{
	constexpr int caveBackgroundOffset = -8000; // How deep down until the cave background shows
	const int lightCamLeft = (camera.getWidth() - bgLight.w) / 2;
	constexpr int lightCamTop = 20;
	constexpr int hillsOffset = -100;
	const double timeOverMax = static_cast<double>(time.time) / static_cast<double>(time.max_time);
	const int horCircle = camera.getWidth() * .35;
	const int vertCircle = camera.getHeight() / 4;
    constexpr SDL_Color dayColor{106, 164, 222, 255};

    // Window size
	winWidth = camera.getWidth();
	winHeight = camera.getHeight();
    
	// Update bgShine
	bgShineSrc.x = 0;
	bgShineSrc.y = 0;
	bgShineSrc.w = bgShine.w;
	bgShineSrc.h = bgShine.h;

	bgShineDest.x = 0;
	bgShineDest.y = 0;
	bgShineDest.w = camera.getWidth();
	bgShineDest.h = camera.getHeight();

	// Update bgCloud
	bgCloudOffset.x = camera.x / 10;
	bgCloudOffset.y = camera.y / 30;

	// Update hills
	bgHillsOffset.x = camera.x / 15;
	bgHillsOffset.y = (camera.getHeight() / 2) + hillsOffset;

	// Enable the cave background is camera is low enough
	showCaveBackground = camera.y < caveBackgroundOffset;
	bgCave.x = camera.x / 10;
	bgCave.y = camera.y / 10;

	//********************************
	//  Handle time
	//********************************

	// Transition to Morning
	// Else if, transition to night
	// Else (switch-case), set brightness to time state 
	if (time.time > time.morning_time - time.morning_offset &&
		time.time <= time.morning_time)
    {		
	    double distance = (static_cast<double>(time.time) -
						   static_cast<double>(time.morning_time) +
						   static_cast<double>(time.morning_offset)) / time.morning_offset;
        
		skyColor.r = distance * dayColor.r;
		skyColor.g = distance * dayColor.g;
		skyColor.b = distance * dayColor.b;
	}
    else if (time.time > time.night_time - time.night_offset &&
             time.time <= time.night_time)
    {
		double distance = abs(static_cast<double>(time.time) -
						   static_cast<double>(time.night_time)) / time.night_offset;
		skyColor.r = distance * dayColor.r;
		skyColor.g = distance * dayColor.g;
		skyColor.b = distance * dayColor.b;
	}
    else
    {
		switch(time.state)
        {
		case TIME_DAY:
			skyColor.r = dayColor.r;
			skyColor.g = dayColor.g;
			skyColor.b = dayColor.b;
			break;
		case TIME_NIGHT:
			skyColor.r = 0;
			skyColor.g = 0;
			skyColor.b = 0;
			break;
		default:
			break;
		}
	}
	
	// Update sun/moon
	bgLightSrc.x = 0;
	bgLightSrc.y = 0;
	bgLightSrc.w = bgLight.w;
	bgLightSrc.h = bgLight.h;

    // Next line is position of light, then sign gets flipped and multiplied to increase size
	bgLightDest.x = lightCamLeft + -sin(timeOverMax * (M_PI*2)) * horCircle;
	bgLightDest.y = bgHillsOffset.y + lightCamTop + cos(timeOverMax * (M_PI * 2)) * vertCircle;
	bgLightDest.w = bgLight.w;
	bgLightDest.h = bgLight.h;

	bgMoonSrc.x = 0;
	bgMoonSrc.y = 0;
	bgMoonSrc.w = bgLight.w;
	bgMoonSrc.h = bgLight.h;

    // Same as last statement, but add 1 half to sin function
	bgMoonDest.x = lightCamLeft + -sin((timeOverMax+.50) * (M_PI*2)) * horCircle;
	bgMoonDest.y = bgHillsOffset.y + lightCamTop + cos((timeOverMax+.50) * (M_PI * 2)) * vertCircle;
	bgMoonDest.w = bgLight.w;
	bgMoonDest.h = bgLight.h;
}

void BackgroundOverworld::render(SDL_Renderer* renderer, TextureHandler& textures) {
    constexpr int cloudOffset = 240;
    constexpr int hillOffset = 0;
	const int afterHillsTop = bgHillsOffset.y + bgHills.h + hillOffset;

    
    bgCaveOpacity.value = showCaveBackground ? 255 : 0;

	// Render sky
	SDL_Rect sky{0, 0, winWidth, winHeight};
    SDL_SetRenderDrawColor(renderer, skyColor.r, skyColor.g, skyColor.b, 255);
    SDL_RenderFillRect(renderer, &sky);
	
	// Render bgShine
    SDL_RenderCopy(renderer, textures.getTexture(13), &bgShineSrc, &bgShineDest);

	// Render sun/moon
    SDL_RenderCopy(renderer, textures.getTexture(20), &bgLightSrc, &bgLightDest);
    SDL_RenderCopy(renderer, textures.getTexture(21), &bgMoonSrc, &bgMoonDest);

	// Repeatedly render clouds and hills
    Generic::Render::renderRepeating(renderer, textures, 14, winWidth, winHeight, bgCloudOffset.x, bgCloudOffset.y,
					 bgCloud.w, bgCloud.h, 60, cloudOffset);
    Generic::Render::renderRepeating(renderer, textures, 15, winWidth, winHeight, bgHillsOffset.x, bgHillsOffset.y,
					 bgHills.w, bgHills.h, 0, hillOffset);

	SDL_Rect after{0, afterHillsTop, winWidth, winHeight-afterHillsTop};
    SDL_SetRenderDrawColor(renderer, 131, 131, 131, 255);
    SDL_RenderFillRect(renderer, &after);
    
	if (bgCaveOpacity.get() > 2) {
        SDL_Texture* tex = textures.getTexture(17);
        SDL_SetTextureAlphaMod(tex, static_cast<int>(bgCaveOpacity.value));

        Generic::Render::renderRepeating(renderer, textures, 17, bgCave.x, bgCave.y,
						 bgCaveBlock.w, bgCaveBlock.h, 0, 0, true, constants::blockImgSize, constants::blockImgSize);

		// Reset opacity for future objects
        SDL_SetTextureAlphaMod(tex, 255);
	}
}

