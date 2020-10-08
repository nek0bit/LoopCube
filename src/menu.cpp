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

Menu::Menu(SDL_Renderer* renderer,
        TextureHandler &textures,
        EventHandler &events,
        int* WINDOW_W,
        int* WINDOW_H) : state{0}, BLOCK_S{40}, BUTTON_W{200}, WINDOW_W{WINDOW_W},
												  WINDOW_H{WINDOW_H}, shift{BLOCK_S}, pad_left{180},
												  header{nullptr} {
    this->renderer = renderer;
    this->textures = &textures;
    this->events = &events;
	
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
        button_group[i] = std::unique_ptr<Button>(new Button(i, textures, 0, 30+(offset_y*i), BUTTON_W));
        button_group[i]->set_text(renderer, option_str[i]);
    }

	//************************************************
	// Setup options
	//************************************************
	back_button = std::unique_ptr<Button>(new Button(-1, textures, 0, 30, 150));
	back_button->set_text(renderer, "<- Back");

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
	random_block = Item(rand_id, textures, renderer);

	//************************************************
	// Setup paragraph string
	//************************************************
	for (auto &i: constants::content) {
		p_string += i + "\n";
	}
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
    // Set background
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 255);

	if (state == MAIN_MENU) {
		auto mouse_pos = events->get_mouse_pos();
		for (auto &i: button_group) {
			i->set_x( (*WINDOW_W/2) + 30 );
			i->update(*events);
		}
	} else if (state == CONFIG_MENU) {
		int left = ( (*WINDOW_W/2) + 30 );
		int top = 80;
		int gap = 50;
		back_button->set_x( (*WINDOW_W/2) + 30 );
		back_button->update(*events);
		for (size_t i = 0; i < c_elements.size(); ++i) {
			c_elements[i]->set_x(left);
			c_elements[i]->set_y(top+(gap*i));
			c_elements[i]->update(*events);

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
    // Render the background in a tile manner with animations
    const int BLOCK_S = 40;
    const int MAX_X = (*WINDOW_W + BLOCK_S*2)/BLOCK_S;
    const int MAX_Y = (*WINDOW_H + BLOCK_S*2)/BLOCK_S;
    // Create a grid of tiles as the background
    for (int i = -1; i < MAX_X; ++i) {
        for (int j = -1; j < MAX_Y; ++j) {
            SDL_Rect block{
                i*BLOCK_S - shift.get_frame(),
                j*BLOCK_S - shift.get_frame(),
                BLOCK_S, BLOCK_S};
            SDL_Rect src{0, 0, 16, 16};

            // Draw the tile
            SDL_RenderCopy(renderer, textures->get_texture(2), &src, &block);
        }
    }
}

void Menu::set_state(int state) {
	update(false); // If we don't update before we change state it thinks that the button is
	          // forever pressed (or the caller of this function in general)
	this->state = state;
	// Update again before we render, or else things may looks strange for a frame
	update(false);
}

void Menu::render_sidebar() {
	const int mid_left = (*WINDOW_W/2)-pad_left;
	const int content_left = mid_left-100;
	
	// Draw line
	SDL_Rect line;
	line.x = mid_left+pad_left;
	line.y = 30;
	line.w = 2;
	line.h = 400;

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderFillRect(renderer, &line);

	// Lets render a random block and some text
	random_block.draw(content_left, 35, 60, 60);

	// White color
	SDL_Color color;
	color.r = 255; color.g = 255; color.b = 255; color.a = 255;
	
	if (header == nullptr) {
		header = std::unique_ptr<Text>(new Text(renderer, constants::header, color, constants::header_font));  
	} else {
		header->draw(content_left+70, 40);
	}

	if (paragraph == nullptr) {
		paragraph = std::unique_ptr<Text>(new Text(renderer, p_string, color, constants::paragraph_font, 400));
	} else {
		paragraph->draw(content_left+5, 120);
	}
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

