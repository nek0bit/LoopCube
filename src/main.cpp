#include "main.hpp"
int main(int argc, char* argv[]) {
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
		
#ifdef _3DS
		svcSleepThread(frame_del - time_taken.count());
#else			
		std::this_thread::sleep_for(std::chrono::milliseconds{frame_del - time_taken.count()});
#endif
	}
	game.free();

	return 0;
}
