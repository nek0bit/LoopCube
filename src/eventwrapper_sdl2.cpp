#include "eventwrapper_sdl2.hpp"

EventWrapper_SDL2::EventWrapper_SDL2() : EventWrapper{} {
	key_mapping = {
		SDL_SCANCODE_W, // UP
		SDL_SCANCODE_D, // RIGHT
		SDL_SCANCODE_S, // DOWN
		SDL_SCANCODE_A, // LEFT
		SDL_SCANCODE_I, // INVENTORY
		SDL_SCANCODE_1, // Slot 1
		SDL_SCANCODE_2, // Slot 2
		SDL_SCANCODE_3, // Slot 3
		SDL_SCANCODE_4, // Slot 4
		SDL_SCANCODE_5, // Slot 5
		SDL_SCANCODE_6, // Slot 6
		SDL_SCANCODE_7, // Slot 7
		SDL_SCANCODE_8, // Slot 8
		SDL_SCANCODE_9, // Slot 9
		SDL_SCANCODE_0, // Slot 0
		SDL_SCANCODE_F8, // DEBUG
		SDL_SCANCODE_C // SPAWN ENTITY
	};
	button_mapping = {
		SDL_CONTROLLER_BUTTON_DPAD_UP,
		SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		SDL_CONTROLLER_BUTTON_X,
		SDL_CONTROLLER_BUTTON_LEFTSTICK,
		SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		SDL_CONTROLLER_BUTTON_START,
		SDL_CONTROLLER_BUTTON_Y,
		SDL_CONTROLLER_BUTTON_GUIDE
	};

	resize_input_mappings();
	init(); // Seperation
}

EventWrapper_SDL2::~EventWrapper_SDL2() {}

void 

void EventWrapper_SDL2::update_controllers() {
	// NOTE this only works with the furthest port priority controller
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		this->controller = SDL_JoystickOpen(i);
		if (this->controller == NULL) {
			std::cout << "[Error] Failed to open SDL2 joystick!" << std::endl;
		} else {
			std::cout << "[Notice] Controller attached" << std::endl;
		}
	}
}

// TODO work on moving more functions from SDL2
