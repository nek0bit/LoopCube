#include "eventwrapper.hpp"

EventWrapper::EventWrapper()
    : quit{false},
      vmouse{0, 0, 0, 0},
      controller{nullptr},
      text_mode{false}
{
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

    resizeInputStates();
}

EventWrapper::~EventWrapper() {}

void EventWrapper::resizeInputStates()
{
    key_state.resize(key_mapping.size());
    button_state.resize(button_mapping.size());
}

void EventWrapper::update_controllers() {
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

void EventWrapper::listen() {
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
	
	if (vmouse.down == 1) vmouse.clicked = 0;

	if (vmouse.x < 0) {
		vmouse.x = 1;
	}
	if (vmouse.y < 0) {
		vmouse.y = 1;
	}

	/* TODO move this bit all into it's own function. Controllers are a seperate thing
	 * so it's best we move it all out of the way; same for the keyboard stuff
	 */
	if (controller != nullptr) {
		auto axis_hor = SDL_JoystickGetAxis(controller, 0);
		auto axis_ver = SDL_JoystickGetAxis(controller, 1);

		int deadzone = 3000;
		if (axis_hor < deadzone || axis_hor > deadzone || axis_ver < deadzone || axis_ver > deadzone) {
			vmouse.x += axis_hor / 2048;
			vmouse.y += axis_ver / 2048;
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
			case SDL_TEXTINPUT:
			if (text_mode) {
				text_mode_buffer += event.text.text;
			}
			break;
		case SDL_KEYDOWN:
			for (auto exc: exceptions) {
				if (keystate[key_mapping[exc]]) {
					key_state[exc] = 1;
				}
			}
			if (text_mode && event.key.keysym.sym == SDLK_BACKSPACE) {
				if (text_mode_buffer.length() != 0) {
					text_mode_buffer.pop_back();
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			vmouse.down = event.button.button;
			vmouse.clicked = event.button.button;
			break;
		case SDL_MOUSEBUTTONUP:
			vmouse.down = 0;
			vmouse.clicked = 0;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&vmouse.x, &vmouse.y);
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
						vmouse.down = 1;
						vmouse.clicked = 1;
					}
					switch(i) {
					case 5:
						quit = true;
						break;
					case 6:
						vmouse.down = 1;
						vmouse.clicked = 1;
						break;
					case 7:
						vmouse.down = 3;
						vmouse.clicked = 3;
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
						vmouse.down = 0;
						vmouse.clicked = 0;
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

void EventWrapper::enable_text_mode() { text_mode = true; }
void EventWrapper::disable_text_mode() { text_mode = false; }
void EventWrapper::set_text_mode_buffer(std::string str) { text_mode_buffer = str; }
void EventWrapper::clear_text_mode_buffer() { set_text_mode_buffer(""); }
