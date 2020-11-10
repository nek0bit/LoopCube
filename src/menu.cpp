#include "menu.hpp"

enum STATE {
	MAIN_MENU,
	CONFIG_MENU
};

enum CONFIG_ID {
	CB_RENDER_SHADOWS,
	CB_RENDER_PARTICLES,
	CB_SHOW_CHUNK_DEBUG,
};

Menu::Menu(GraphicsWrapper* renderer,
		   EventWrapper*& events) : state{0}, BLOCK_S{40}, BUTTON_W{200}, shift{BLOCK_S},
									back{40, 40, 400, 400}, pad_left{180} {
	this->renderer = renderer;
	this->events = events;
	
	//************************************************
	// Resize option_state to match option_str size
	//************************************************
	this->option_str = {"Play", "About", "Config", "Exit"};
	option_state.resize(option_str.size());
	button_group.resize(option_str.size());

	//************************************************
	// Setup buttons in main menu
	//************************************************
	const int offset_y = 48;
	
	for (size_t i = 0; i < button_group.size(); ++i) {
		button_group[i] = std::unique_ptr<Button>(new Button(i, 0, 30+(offset_y*i), BUTTON_W));
		button_group[i]->set_text(option_str[i]);
	}

	//************************************************
	// Setup options
	//************************************************
	back_button = std::unique_ptr<Button>(new Button(-1, 0, 30, 150));
	back_button->set_text("<- Back");

	// Setup checkboxes
	Checkbox* show_shadows = new Checkbox(CB_RENDER_SHADOWS, "Show Shadows", 0, 0, 30);
	Checkbox* show_particles = new Checkbox(CB_RENDER_PARTICLES, "Show Particles", 0, 0, 30);
	Checkbox* show_info = new Checkbox(CB_SHOW_CHUNK_DEBUG, "Show Chunk Debug Info", 0, 0, 30);
	if (constants::config.get_int(CONFIG_SHOW_SHADOWS) == 1) show_shadows->check();
	if (constants::config.get_int(CONFIG_SHOW_PARTICLES) == 1) show_particles->check();
	if (constants::config.get_int(CONFIG_SHOW_CHUNK_DEBUG) == 1) show_info->check();

	// Insert checkboxes
	c_elements.push_back(show_shadows);
	c_elements.push_back(show_particles);
	c_elements.push_back(show_info);

	//************************************************
	// Setup random block
	//************************************************
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, constants::block_info.size()-1);

	int rand_id = constants::block_info.at(dist(rng)).get_id();
	random_block = Item(rand_id);

	//************************************************
	// Setup paragraph string
	//************************************************
	for (auto &i: constants::content) {
		p_string += i + "\n";
	}
	header = Text{constants::header};
	paragraph = Text{p_string};
}

Menu::~Menu() {
	std::cout << "[Menu] Clearing config elements" << std::endl;
	// Delete all config elements
	for (auto i: c_elements) {
		delete i;
	}
}

void Menu::update_config_elements(int id, int value) {
	switch(id) {
	case CB_RENDER_SHADOWS:
		constants::config.set(CONFIG_SHOW_SHADOWS, value);
		break;
	case CB_RENDER_PARTICLES:
		constants::config.set(CONFIG_SHOW_PARTICLES, value);
		break;
	case CB_SHOW_CHUNK_DEBUG:
		constants::config.set(CONFIG_SHOW_CHUNK_DEBUG, value);
		break;
	default:
		break;
	}
}

void Menu::update(bool update_animations) {
	auto win = renderer->screen_size();
	// Set background
	if (state == MAIN_MENU) {
		for (auto &i: button_group) {
			i->set_x( (win[0]/2) + 30 );
			i->update(events);
		}
	} else if (state == CONFIG_MENU) {
		int left = ( (win[0]/2) + 30 );
		int top = 80;
		int gap = 50;
		back_button->set_x( (win[0]/2) + 30 );
		back_button->update(events);
		for (size_t i = 0; i < c_elements.size(); ++i) {
			c_elements[i]->set_x(left);
			c_elements[i]->set_y(top+(gap*i));
			c_elements[i]->update(events);

			c_elements[i]->on_change(update_config_elements);
		}

		if (back_button->get_pressed()) {
			set_state(MAIN_MENU);
		}

		
	}

	// Update animation for background
	if (update_animations) { // Removes stutter if we must call multiple updates in a single frame
		shift.tick();
	}
}

void Menu::render_background() {
	auto win = renderer->screen_size();
	// Render the background in a tile manner with animations
	const int BLOCK_S = 40;
	const int MAX_X = (win[0] + BLOCK_S*2)/BLOCK_S;
	const int MAX_Y = (win[1] + BLOCK_S*2)/BLOCK_S;
	// Create a grid of tiles as the background
	for (int i = -1; i < MAX_X; ++i) {
		for (int j = -1; j < MAX_Y; ++j) {
		    Rect block{
				i*BLOCK_S - shift.get_frame(),
				j*BLOCK_S - shift.get_frame(),
				BLOCK_S, BLOCK_S};
			Rect src{0, 0, 16, 16};

			// Draw the tile
			renderer->render(src, block, 2);
		}
	}

	back.render(renderer);
}

void Menu::set_state(int state) {
	update(false); // If we don't update before we change state it thinks that the button is
	// forever pressed (or the caller of this function in general)
	this->state = state;
	// Update again before we render, or else things may looks strange for a frame
	update(false);
}

void Menu::render_sidebar() {
	const int mid_left = (renderer->screen_size()[0]/2)-pad_left;
	const int content_left = mid_left-100;
	
	// Draw line
    Rect line{mid_left+pad_left, 30, 2, 400};

	renderer->render_rect(line, Color{200, 200, 200, 255});

	// Lets render a random block and some text
	random_block.draw(renderer, content_left, 35, 60, 60);

	// White color
    Color color;
	color.r = 255; color.g = 255; color.b = 255; color.a = 255;
	
	header.render(renderer, content_left+70, 45, 32, true);

	paragraph.render(renderer, content_left+5, 120, 20, true);
}

int Menu::get_pressed() {
	for (auto &i: button_group) {
		if (i->get_pressed()) {
			return i->get_id();
		}
	}
	return -1;
}


void Menu::render_main_menu() {
	// Render all buttons
	for (auto &i: button_group) {
		i->render(renderer);
	}
}

void Menu::render_config_menu() {
	back_button->render(renderer);
	// Render all elements, no matter the type
	for (auto &i: c_elements) {
		i->render(renderer);
	}
}

void Menu::render() {
	render_background();
	render_sidebar();

	if (state == MAIN_MENU) {		
		render_main_menu();
	} else if (state == CONFIG_MENU) {
		render_config_menu();
	}
}

