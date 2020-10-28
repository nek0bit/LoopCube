#include "main.hpp"
int main() {
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
		const int FPS = 60;
		const int frame_del = 1000 / FPS;

		Game game{};

		game.init(false);
		
		while(game.running()) {
		    auto time_before = std::chrono::system_clock::now();
		    
			game.update();
			game.render();
			game.event_handler();

			auto time_after = std::chrono::system_clock::now();

			auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(time_after - time_before);
			std::cout << time_taken.count() << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds{frame_del - time_taken.count()});
		}
		game.free();
#ifdef __SWITCH__
		break;
	}
	
	romfsExit();
#endif

	SDL_Quit();

	return 0;
}
