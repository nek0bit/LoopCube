#include "eventhandler.hpp"

EventHandler::EventHandler()
    : quit{false},
	  keys_set{SDL_SCANCODE_W,
	SDL_SCANCODE_D,
	SDL_SCANCODE_S,
	SDL_SCANCODE_A,
	SDL_SCANCODE_I,
	SDL_SCANCODE_1,
	SDL_SCANCODE_2,
	SDL_SCANCODE_3,
	SDL_SCANCODE_4,
	SDL_SCANCODE_5,
	SDL_SCANCODE_6,
	SDL_SCANCODE_7,
	SDL_SCANCODE_8,
	SDL_SCANCODE_9,
	SDL_SCANCODE_0,
	SDL_SCANCODE_F8,
	SDL_SCANCODE_C},
	  buttons_set{SDL_CONTROLLER_BUTTON_MAX,
	SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	SDL_CONTROLLER_BUTTON_X,
	SDL_CONTROLLER_BUTTON_LEFTSTICK,
	SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	SDL_CONTROLLER_BUTTON_START,
	SDL_CONTROLLER_BUTTON_Y,
	SDL_CONTROLLER_BUTTON_GUIDE},
#ifdef __SWITCH__
	  conID{hidGetHandheldMode() ? CONTROLLER_HANDHELD : CONTROLLER_PLAYER_1},
#endif
	  mouse_down{0}, mouse_clicked{0}, controller{NULL} {
    state.resize(keys_set.size());
    button_state.resize(buttons_set.size());
    
    // Check if on a switch, then use libnx hdl
    #ifdef __SWITCH__
    hiddbgInitialize();
    #endif
}

void EventHandler::open_controllers() {

    // Check Joysticks
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        this->controller = SDL_JoystickOpen(i);
        if (this->controller == NULL) {
            std::cout << "Warning: Unable to open game controller" << std::endl;
        } else {
            std::cout << "Controller registered!" << std::endl;
        }
    }
}

EventHandler::~EventHandler() {
    std::cout << "Closing events..." << std::endl;
    #ifdef __SWITCH__
    hiddbgExit();
    #endif
}

void EventHandler::listen() {
    std::vector<int> exceptions{4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    std::vector<int> controller_exceptions{6, 8};

    // Disable Exceptions
    for (auto exc: exceptions) {
        if (state[exc] == 1) {
            state[exc] = 0;
        }
    }

    for (auto exc: controller_exceptions) {
        if (button_state[exc] == 1) {
            button_state[exc] = 0;
        }
    }
    
    if (mouse_down == 1) mouse_clicked = 0;

    if (mouse_x < 0) {
        mouse_x = 1;
    }
    if (mouse_y < 0) {
        mouse_y = 1;
    }

    /* TODO move this bit all into it's own function. Controllers are a seperate thing
     * so it's best we move it all out of the way; same for the keyboard stuff
     */
    if (controller != NULL) {
        #ifdef __SWITCH__
        // Switch doesn't like SDL joysticks (but recognizes buttons fine), instead, let's just use libnx for this
        hidScanInput();
        
        if (hidIsControllerConnected(conID)) {
            // TODO maybe add deadzone for controllers with slight drift
            JoystickPosition tmp[2];
            hidJoystickRead(&tmp[0], conID, JOYSTICK_LEFT);
            hidJoystickRead(&tmp[1], conID, JOYSTICK_RIGHT);
                
            // Right joystick
            mouse_x += tmp[1].dx / 2048;
            // JoystickPosition.dy is inverted
            mouse_y += (tmp[1].dy*-1) / 2048;
            
            // Left joystick
            mouse_x += tmp[0].dx / 2048;
            mouse_y += (tmp[0].dy*-1) / 2048;
        }
        #else
        /* Note: This works on the Wii U gamepad, but probably not for other controllers
         * Soon there will be a menu so the user can setup their controller and it's
         * axis's so I don't have to just guess them, which would be a pain.
         *
         * maybe one day I will also implement auto configuration, but in the end it mostly
         * causes issues with different controllers. So it's best I allow the user to manually
         * set them up.
         */
        auto axis_hor = SDL_JoystickGetAxis(controller, 2);
        auto axis_ver = SDL_JoystickGetAxis(controller, 3);

        int deadzone = 3000;
        if (axis_hor < deadzone || axis_hor > deadzone || axis_ver < deadzone || axis_ver > deadzone) {
            mouse_x += axis_hor / 2048;
            mouse_y += axis_ver / 2048;
        }
        #endif
    }

	#ifdef __SWITCH__
	if (conID == CONTROLLER_HANDHELD) {
		touchPosition touch;
		u32 touch_count = hidTouchCount();
	    for (u32 i = 0; i < touch_count; ++i) {
			hidTouchRead(&touch, i);

			mouse_x = touch.px;
			mouse_y = touch.py;
			if (!mouse_down) mouse_clicked = true;
			mouse_down = true;
		}
	}
	#endif

    while (SDL_PollEvent(&event)) {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        // Set state for each key
        for (size_t i = 0; i < keys_set.size(); ++i) {
            // Check exceptions; if found, skip.
            bool check = false;
            for (auto exc: exceptions) {
                if (static_cast<size_t>(exc) == i) {
                    check = true;
                    break;
                }
            }
            if (check == true) continue;

            if (keystate[keys_set[i]]) {
                state[i] = 1;
            } else {
                state[i] = 0;
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
                    if (keystate[keys_set[exc]]) {
                        state[exc] = 1;
                    }
                }
                if (text_mode && event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (text_mode_buffer.length() != 0) {
                        text_mode_buffer.pop_back();
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_down = event.button.button;
                mouse_clicked = event.button.button;
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_down = 0;
                mouse_clicked = 0;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouse_x, &mouse_y);
                break;
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_JOYBUTTONDOWN:
                for (size_t i = 0; i < buttons_set.size(); ++i) {
                    if (event.jbutton.button == buttons_set[i]) {
                        button_state[i] = 1;
                        if (i == 5) {
                            quit = true;
                        }
                        if (i == 6) {
                            mouse_down = 1;
                            mouse_clicked = 1;
                        }
						switch(i) {
						case 5:
							quit = true;
							break;
						case 6:
							mouse_down = 1;
							mouse_clicked = 1;
							break;
						case 7:
							mouse_down = 3;
							mouse_clicked = 3;
							break;
						default:
							break;
						}
                    }
                }
                break;
            case SDL_JOYBUTTONUP:
                for (size_t i = 0; i < buttons_set.size(); ++i) {
                    if (event.jbutton.button == buttons_set[i]) {
                        button_state[i] = 0;
						switch(i) {
						case 6:
						case 7:
							mouse_down = 0;
							mouse_clicked = 0;
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

std::vector<int> EventHandler::get_state() {
    return state;
}

std::vector<int> EventHandler::get_button_state() {
    return button_state;
}

std::vector<SDL_Scancode> EventHandler::get_keys_set() {
    return keys_set;
}

std::array<int, 2> EventHandler::get_mouse_pos() {
    std::array<int, 2> i{mouse_x, mouse_y};
    return i;
}

int EventHandler::get_mouse_down() {
    return mouse_down;
}

int EventHandler::get_mouse_clicked() {
    return mouse_clicked;
}

SDL_Event EventHandler::get_event() {
    return event;
}

bool EventHandler::get_quit() {
    return quit;
}

// Text mode

void EventHandler::enable_text_mode() {
    text_mode = true;
}

void EventHandler::disable_text_mode() {
    text_mode = false;
}

void EventHandler::clear_text_mode_buffer() {
    set_text_mode_buffer("");
}

void EventHandler::set_text_mode_buffer(std::string str) {
    text_mode_buffer = str;
}

std::string& EventHandler::get_text_buffer() {
    return text_mode_buffer;
}
