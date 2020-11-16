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

#ifdef _SDL2
	Uint32 frame;
	int frame_time;
	
	while(game.running()) {
		frame = SDL_GetTicks();
#else
		
	while(game.running()) {
		auto time_before = std::chrono::system_clock::now();
#endif
		
	game_handles(game);

#ifdef _SDL2
	    frame_time = SDL_GetTicks() - frame;

		if (frame_time < frame_del) {
			SDL_Delay(frame_del - frame_time);
		}
	}
#else
	    auto time_after = std::chrono::system_clock::now();
		
		auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(time_after - time_before);
		
		std::this_thread::sleep_for(std::chrono::milliseconds{frame_del - time_taken.count()});
	}
#endif
		
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
