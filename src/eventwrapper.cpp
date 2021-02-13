#include "eventwrapper.hpp"

EventWrapper::EventWrapper()
    : quit{false},
      vmouse{0, 0, 0, 0, 0},
      controller{nullptr},
      textMode{false},
#ifdef __SWITCH__
      conID{hidGetHandheldMode() ? CONTROLLER_HANDHELD : CONTROLLER_PLAYER_1}
#endif
{
	keyMapping = {
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
		SDL_SCANCODE_C, // SPAWN ENTITY
        SDL_SCANCODE_ESCAPE // Exit
	};
	buttonMapping = {
#ifdef __SWITCH__
        // Nintendo switch defaults
        SDL_CONTROLLER_BUTTON_MAX,
        SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        SDL_CONTROLLER_BUTTON_X,
        SDL_CONTROLLER_BUTTON_LEFTSTICK,
        SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        SDL_CONTROLLER_BUTTON_START,
        SDL_CONTROLLER_BUTTON_Y,
        SDL_CONTROLLER_BUTTON_GUIDE
#else
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
#endif
	};

    resizeInputStates();

#ifdef __SWITCH__
    hiddbgInitialize();
#endif
}

EventWrapper::~EventWrapper()
{
    SDL_JoystickClose(this->controller);

#ifdef __SWITCH__
    hiddbgExit();
#endif
}

void EventWrapper::resizeInputStates()
{
    keyState.resize(keyMapping.size());
    buttonState.resize(buttonMapping.size());
}

void EventWrapper::updateControllers() {
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
	std::vector<int> exceptions{4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
	std::vector<int> controller_exceptions{6, 8};

	
	// Disable Exceptions
	for (auto exc: exceptions) {
		if (keyState[exc] == 1) {
			keyState[exc] = 0;
		}
	}

	for (auto exc: controller_exceptions) {
		if (buttonState[exc] == 1) {
			buttonState[exc] = 0;
		}
	}
	// End TODO
	
	if (vmouse.down == 1) vmouse.clicked = 0;
    vmouse.scroll = 0;

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
#ifdef __SWITCH__
        // Switch doesn't like SDL joysticks (but recognizes buttons fine), instead, let's just use libnx for this
		hidScanInput();
		
		if (hidIsControllerConnected(conID)) {
			// TODO maybe add deadzone for controllers with slight drift
			JoystickPosition tmp[2];
			hidJoystickRead(&tmp[0], conID, JOYSTICK_LEFT);
			hidJoystickRead(&tmp[1], conID, JOYSTICK_RIGHT);
				
			// Right joystick
			vmouse.x += tmp[1].dx / 2048;
			// JoystickPosition.dy is inverted
			vmouse.y += -tmp[1].dy / 2048;
			
			// Left joystick
			vmouse.x += tmp[0].dx / 2048;
			vmouse.y += -tmp[0].dy / 2048;
		}
#else
		auto axis_hor = SDL_JoystickGetAxis(controller, 0);
		auto axis_ver = SDL_JoystickGetAxis(controller, 1);

		int deadzone = 3000;
		if (axis_hor < deadzone || axis_hor > deadzone || axis_ver < deadzone || axis_ver > deadzone) {
			vmouse.x += axis_hor / 2048;
			vmouse.y += axis_ver / 2048;
		}
#endif
	}
    
#ifdef __SWITCH__
    if (conID == CONTROLLER_HANDHELD) {
        touchPosition touch;
        u32 touch_count = hidTouchCount();
        for (u32 i = 0; i < touch_count; ++i) {
            hidTouchRead(&touch, i);

            vmouse.x = touch.px;
            vmouse.y = touch.py;
            if (!vmouse.down) vmouse.clicked = true;
            vmouse.down = true;
        }
    }
#endif

	while (SDL_PollEvent(&event)) {
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		// Set state for each key
		for (size_t i = 0; i < keyMapping.size(); ++i) {
			// Check exceptions; if found, skip.
			bool check = false;
			for (auto exc: exceptions) {
				if (static_cast<size_t>(exc) == i) {
					check = true;
					break;
				}
			}
			if (check == true) continue;

			if (keystate[keyMapping[i]]) {
				keyState[i] = 1;
			} else {
				keyState[i] = 0;
			}
		}

		switch(event.type) {
			case SDL_TEXTINPUT:
			if (textMode) {
				textModeBuffer += event.text.text;
			}
			break;
		case SDL_KEYDOWN:
			for (auto exc: exceptions) {
				if (keystate[keyMapping[exc]]) {
					keyState[exc] = 1;
				}
			}
			if (textMode && event.key.keysym.sym == SDLK_BACKSPACE) {
				if (textModeBuffer.length() != 0) {
					textModeBuffer.pop_back();
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
        case SDL_MOUSEWHEEL:
            vmouse.scroll = event.wheel.y;
            break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&vmouse.x, &vmouse.y);
			break;
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_JOYBUTTONDOWN:
			for (size_t i = 0; i < buttonMapping.size(); ++i) {
				if (event.jbutton.button == buttonMapping[i]) {
					buttonState[i] = 1;
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
			for (size_t i = 0; i < buttonMapping.size(); ++i) {
				if (event.jbutton.button == buttonMapping[i]) {
					buttonState[i] = 0;
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
