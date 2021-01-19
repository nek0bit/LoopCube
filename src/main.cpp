#include "main.hpp"

void gameHandles(Game& game) {
	game.update();
	game.render();
	game.eventHandler();
}

void gameLoop() {
    Timer timer{100};
	Game game{timer};
	
	game.init(false);
	
	while(game.isRunning) {
        timer.setTime();

        std::cout << "Time/ms: " << timer.deltaTime.ms << std::endl;
        std::cout << "Time/s: " << timer.deltaTime.s << std::endl;
        
        gameHandles(game);
        
        int calc = timer.calcSleep();
        if (calc > 0) SDL_Delay(calc);
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
