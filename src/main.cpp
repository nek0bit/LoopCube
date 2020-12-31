#include "main.hpp"

void game_handles(Game& game) {
	game.update();
	game.render();
	game.event_handler();
}

void game_loop() {
	const int FPS = 60;
	const int frame_del = 1000 / FPS;
	
	Game game{};
	
	game.init(false);

	Uint32 frame;
	int frame_time;
	
	while(game.running()) {
		frame = SDL_GetTicks();
		
        game_handles(game);

	    frame_time = SDL_GetTicks() - frame;

		if (frame_time < frame_del) {
			SDL_Delay(frame_del - frame_time);
		}
	}
		
	game.free();
}

int main(int argc, char* argv[]) {
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

	game_loop();

// End main applet loop for nintendo switch
#ifdef __SWITCH__
		break;
	}
	
	romfsExit();
#endif

	return 0;
}
