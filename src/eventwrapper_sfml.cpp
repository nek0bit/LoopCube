#ifdef INPUT_BACKEND_SFML
#include "eventwrapper_sfml.hpp"

EventWrapper_SFML::EventWrapper_SFML(sf::RenderWindow* screen) : screen{screen} {
	key_mapping = {
		sf::Keyboard::W,
		sf::Keyboard::D,
		sf::Keyboard::S,
		sf::Keyboard::A,
		sf::Keyboard::I,
		sf::Keyboard::Num1,
		sf::Keyboard::Num2,
		sf::Keyboard::Num3,
		sf::Keyboard::Num4,
		sf::Keyboard::Num5,
		sf::Keyboard::Num6,
		sf::Keyboard::Num7,
		sf::Keyboard::Num8,
		sf::Keyboard::Num9,
		sf::Keyboard::Num0,
		sf::Keyboard::F8,
		sf::Keyboard::C
	};
	button_mapping = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	};

	resize_input_mappings();
	init();
}

EventWrapper_SFML::~EventWrapper_SFML() {}

void EventWrapper_SFML::init() {}

void EventWrapper_SFML::update_controllers() {}

void EventWrapper_SFML::listen() {
	sf::Event event;

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


	while (screen->pollEvent(event)) {


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

			if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key_mapping[i]))) {
				key_state[i] = 1;
			} else {
				key_state[i] = 0;
			}
		}
		
		switch (event.type) {
		case sf::Event::Closed:
			screen->close();
		    quit = true;
			break;
		case sf::Event::KeyPressed:
			for (size_t i = 0; i < key_mapping.size(); ++i) {
				if (event.key.code == key_mapping[i]) {
					key_state[i] = 1;
				}
			}
			break;
		case sf::Event::MouseMoved:
			vmouse_x = event.mouseMove.x;
			vmouse_y = event.mouseMove.y;
			break;
		case sf::Event::MouseButtonPressed:
			switch(event.mouseButton.button) {
			case sf::Mouse::Left:
				vmouse_clicked = 1;
				vmouse_down = 1;
				break;
			case sf::Mouse::Right:
				vmouse_clicked = 3;
				vmouse_down = 3;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonReleased:
		    vmouse_clicked = 0;
			vmouse_down = 0;
			break;
		default:
			break;
		}
	}
}

#endif
