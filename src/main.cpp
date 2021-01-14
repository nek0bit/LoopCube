#include "main.hpp"

void gameHandles(Game& game) {
	game.update();
	game.render();
	game.eventHandler();
}

void gameLoop() {
	constexpr int FPS = 60;
	constexpr int FRAME_DEL = 1000 / FPS;
	
	Game game{};
	
	game.init(false);

	Uint32 frame;
	int frame_time;
	
	while(game.isRunning) {
		frame = SDL_GetTicks();
		
        gameHandles(game);

	    frame_time = SDL_GetTicks() - frame;

		if (frame_time < FRAME_DEL) {
			SDL_Delay(FRAME_DEL - frame_time);
		}
	}
		
	game.free();
}

int main(/* int argc, char* argv[] */) {
// Main applet loop for nintendo switch
#ifdef __SWITCH__	 
	hidScanInput();
	
	Result rc = 0;
	rc = hiddbgInitialize();
	
	if (R_FAILED(rc)) {
		std::cout << "hiddbgInitialize(): " << rc << std::endl;
	}
	
	while(appletMainLoop()) {
		romfsInit();
#endif

	gameLoop();

// End main applet loop for nintendo switch
#ifdef __SWITCH__
		break;
	}
	
	romfsExit();
#endif

	return 0;
}
