#ifdef INPUT_BACKEND_SDL2

#include "eventwrapper_sdl2.hpp"

EventWrapper_SDL2::EventWrapper_SDL2() : EventWrapper{}, controller{nullptr} {
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

void EventWrapper_SDL2::init() {}

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

void EventWrapper_SDL2::listen() {
	// TODO move exceptions from SDL2 into internal class
	std::vector<int> exceptions{4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	std::vector<int> controller_exceptions{6, 8};

	
	// Disable Exceptions
	for (auto exc: exceptions) {
		if (key_state[exc] == 1) {
			key_state[exc] = 0;
		}
	}

	for (auto exc: controller_exceptions) {
		if (button_state[exc] == 1) {
			button_state[exc] = 0;
		}
	}
	// End TODO
	
	if (vmouse_down == 1) vmouse_clicked = 0;

	if (vmouse_x < 0) {
		vmouse_x = 1;
	}
	if (vmouse_y < 0) {
		vmouse_y = 1;
	}

	/* TODO move this bit all into it's own function. Controllers are a seperate thing
	 * so it's best we move it all out of the way; same for the keyboard stuff
	 */
	if (controller != nullptr) {
		auto axis_hor = SDL_JoystickGetAxis(controller, 0);
		auto axis_ver = SDL_JoystickGetAxis(controller, 1);

		int deadzone = 3000;
		if (axis_hor < deadzone || axis_hor > deadzone || axis_ver < deadzone || axis_ver > deadzone) {
			vmouse_x += axis_hor / 2048;
			vmouse_y += axis_ver / 2048;
		}
	}


	while (SDL_PollEvent(&event)) {
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		// Set state for each key
		for (size_t i = 0; i < key_mapping.size(); ++i) {
			// Check exceptions; if found, skip.
			bool check = false;
			for (auto exc: exceptions) {
				if (static_cast<size_t>(exc) == i) {
					check = true;
					break;
				}
			}
			if (check == true) continue;

			if (keystate[key_mapping[i]]) {
				key_state[i] = 1;
			} else {
				key_state[i] = 0;
			}
		}

		switch(event.type) {
			// TODO provide a portable solution to this
			//case SDL_TEXTINPUT:
			//if (text_mode) {
			//	text_mode_buffer += event.text.text;
			//}
			//break;
		case SDL_KEYDOWN:
			for (auto exc: exceptions) {
				if (keystate[key_mapping[exc]]) {
					key_state[exc] = 1;
				}
			}
			//if (text_mode && event.key.keysym.sym == SDLK_BACKSPACE) {
			//	if (text_mode_buffer.length() != 0) {
			//		text_mode_buffer.pop_back();
			//	}
			//}
			break;
		case SDL_MOUSEBUTTONDOWN:
			vmouse_down = event.button.button;
			vmouse_clicked = event.button.button;
			break;
		case SDL_MOUSEBUTTONUP:
			vmouse_down = 0;
			vmouse_clicked = 0;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&vmouse_x, &vmouse_y);
			break;
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_JOYBUTTONDOWN:
			for (size_t i = 0; i < button_mapping.size(); ++i) {
				if (event.jbutton.button == button_mapping[i]) {
					button_state[i] = 1;
					if (i == 5) {
						quit = true;
					}
					if (i == 6) {
						vmouse_down = 1;
						vmouse_clicked = 1;
					}
					switch(i) {
					case 5:
						quit = true;
						break;
					case 6:
						vmouse_down = 1;
						vmouse_clicked = 1;
						break;
					case 7:
						vmouse_down = 3;
						vmouse_clicked = 3;
						break;
					default:
						break;
					}
				}
			}
			break;
		case SDL_JOYBUTTONUP:
			for (size_t i = 0; i < button_mapping.size(); ++i) {
				if (event.jbutton.button == button_mapping[i]) {
					button_state[i] = 0;
					switch(i) {
					case 6:
					case 7:
						vmouse_down = 0;
						vmouse_clicked = 0;
						break;
					default:
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

#endif
