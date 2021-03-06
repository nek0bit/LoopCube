#include "background_overworld.hpp"

BackgroundOverworld::BackgroundOverworld()
	: Background{},
      winWidth{0},
      winHeight{0},
      bgShineSrc{},
      bgShineDest{},
      bgLightSrc{},
      bgLightDest{},
      bgMoonSrc{},
      bgMoonDest{},
      bgCloudOffset{0, 0},
      bgHillsOffset{0, 0},
      bgShine{10, 150},
      bgCloud{450, 100},
      bgHills{450*2, 114*2},
      bgLight{100, 100},
      skyColor{0, 0, 0, 255}
{}

BackgroundOverworld::~BackgroundOverworld()
{}

void BackgroundOverworld::update(const Camera& camera, Time& time)
{
	const int lightCamLeft = (camera.size.w - bgLight.w) / 2;
	constexpr int lightCamTop = 20;
	constexpr int hillsOffset = -100;
	const double timeOverMax = static_cast<double>(time.time) / static_cast<double>(time.maxTime);
	const int horCircle = camera.size.w * .35;
	const int vertCircle = camera.size.h / 4;
    constexpr SDL_Color dayColor{106, 164, 222, 255};

    // Window size
	winWidth = camera.size.w;
	winHeight = camera.size.h;
    
	// Update bgShine
	bgShineSrc.x = 0;
	bgShineSrc.y = 0;
	bgShineSrc.w = bgShine.w;
	bgShineSrc.h = bgShine.h;

	bgShineDest.x = 0;
	bgShineDest.y = 0;
	bgShineDest.w = camera.size.w;
	bgShineDest.h = camera.size.h;

	// Update bgCloud
	bgCloudOffset.x = camera.position.x / 10;
	bgCloudOffset.y = camera.position.y / 30;

	// Update hills
	bgHillsOffset.x = camera.position.x / 15;
	bgHillsOffset.y = (camera.size.h / 2) + hillsOffset;

	//********************************
	//  Handle time
	//********************************

	// Transition to Morning
	// Else if, transition to night
	// Else (switch-case), set brightness to time state 
	if (time.time > time.morningTime - time.morningOffset &&
		time.time <= time.morningTime)
    {		
	    double distance = (static_cast<double>(time.time) -
						   static_cast<double>(time.morningTime) +
						   static_cast<double>(time.morningOffset)) / time.morningOffset;
        
		skyColor.r = distance * dayColor.r;
		skyColor.g = distance * dayColor.g;
		skyColor.b = distance * dayColor.b;
	}
    else if (time.time > time.nightTime - time.nightOffset &&
             time.time <= time.nightTime)
    {
		double distance = abs(static_cast<double>(time.time) -
						   static_cast<double>(time.nightTime)) / time.nightOffset;
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

void BackgroundOverworld::render(const Graphics& graphics) const
{
    constexpr int cloudOffset = 240;
    constexpr int hillOffset = 0;
	const int afterHillsTop = bgHillsOffset.y + bgHills.h + hillOffset;

	// Render sky
	SDL_Rect sky{0, 0, winWidth, winHeight};
    //SDL_SetRenderDrawColor(renderer, skyColor.r, skyColor.g, skyColor.b, 255);
    //SDL_RenderFillRect(renderer, &sky);
	
	// Render bgShine
    //SDL_RenderCopy(renderer, textures.getTexture(TEXTURE_BG_SHINE)->texture, &bgShineSrc, &bgShineDest);

	// Render sun/moon
    //SDL_RenderCopy(renderer, textures.getTexture(TEXTURE_SUN)->texture, &bgLightSrc, &bgLightDest);
    //SDL_RenderCopy(renderer, textures.getTexture(TEXTURE_MOON)->texture, &bgMoonSrc, &bgMoonDest);

	// Repeatedly render clouds and hills
    /*
    Generic::Render::renderRepeating(renderer, textures, TEXTURE_BG_CLOUD_LOOP, winWidth, winHeight, bgCloudOffset.x, bgCloudOffset.y,
					 bgCloud.w, bgCloud.h, 60, cloudOffset);
    Generic::Render::renderRepeating(renderer, textures, TEXTURE_BG_HILLS_HQ, winWidth, winHeight, bgHillsOffset.x, bgHillsOffset.y,
					 bgHills.w, bgHills.h, 0, hillOffset);
    */

	SDL_Rect after{0, afterHillsTop, winWidth, winHeight-afterHillsTop};
    //SDL_SetRenderDrawColor(renderer, 111, 106, 98, 255);
    //SDL_RenderFillRect(renderer, &after);
}

