#include "main.hpp"

void gameLoop(int argc, char* argv[]) {
    Timer timer{120};
	Game game{timer, argc, argv};
	
	game.init(false);
	
	while(game.isRunning) {
        timer.setTime();
        
        game.update();
        game.render();
        game.eventHandler();
        
        int calc = timer.calcSleep();
        if (calc > 0) std::this_thread::sleep_for(std::chrono::milliseconds(timer.calcSleep()));

        timer.calcFPS();
	}
		
	game.free();
}

int main(int argc, char* argv[]) {
// Main applet loop for nintendo switch
#ifdef __SWITCH__	 
	hidScanInput();
	
	Result rc = hiddbgInitialize();
	
	if (R_FAILED(rc)) {
		std::cout << "hiddbgInitialize(): " << rc << std::endl;
	}
	
	while(appletMainLoop()) {
		romfsInit();
#endif

        gameLoop(argc, argv);

// End main applet loop for nintendo switch
#ifdef __SWITCH__
		break;
	}
	
	romfsExit();
#endif

	return 0;
}
