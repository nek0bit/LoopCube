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

Menu::Menu(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, WinSize& winSize)
    : state{0},
      winSize{winSize},
      offset_x{0},
      offset_y{0},
      BLOCK_S{40},
      BUTTON_W{200},
      back_button{nullptr},
      renderer{renderer},
      textures{textures},
      events{events},
      shift{BLOCK_S},
      back{40, 40, 400, 400},
      box_width{660},
      box_height{460},
      prev_mid_left{0},
      mid_left{0},
      content_left{0},
      pad_left{180},
      header{nullptr},
      paragraph{nullptr}
{		
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
		button_group[i] = std::make_unique<Button>(Button(renderer, i, 0, 30+(offset_y*i), BUTTON_W));
		button_group[i]->setText(option_str[i]);
	}

	//************************************************
	// Setup options
	//************************************************
	back_button = std::make_unique<Button>(Button(renderer, -1, 0, 30, 150));
	back_button->setText("Return");

	// Setup checkboxes
	Checkbox* show_shadows = new Checkbox(renderer, CB_RENDER_SHADOWS, "Show Shadows", 0, 0, 30);
	Checkbox* show_particles = new Checkbox(renderer, CB_RENDER_PARTICLES, "Show Particles", 0, 0, 30);
	Checkbox* show_info = new Checkbox(renderer, CB_SHOW_CHUNK_DEBUG, "Show Chunk Debug Info", 0, 0, 30);
	if (constants::config.getInt(CONFIG_SHOW_SHADOWS) == 1) show_shadows->checked = true;
	if (constants::config.getInt(CONFIG_SHOW_PARTICLES) == 1) show_particles->checked = true;
	if (constants::config.getInt(CONFIG_SHOW_CHUNK_DEBUG) == 1) show_info->checked = true;

	// Insert checkboxes
	c_elements.push_back(show_shadows);
	c_elements.push_back(show_particles);
	c_elements.push_back(show_info);

	//************************************************
	// Setup random block
	//************************************************
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, constants::blockInfo.size()-1);

	int rand_id = constants::blockInfo[dist(rng)].id;
	random_block = Item(renderer, rand_id);

	//************************************************
	// Setup paragraph string
	//************************************************
	for (auto &i: constants::content) {
		p_string += i + "\n";
	}
    SDL_Color textColor{255, 255, 255, 255};
	header = new Text(renderer, constants::header, textColor, constants::fontHandler.getFont(5));
	paragraph = new Text(renderer, p_string, textColor, constants::fontHandler.getFont(3), 240);

	// Ensure all elements are properly updated (ex: offsets)
	update();
}

Menu::~Menu() {
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
	offset_x = 0;
	offset_y = (winSize.h/2) - ((box_height)/2);

	const int left = (winSize.w/2) + 30;
	constexpr int top = 80;
	constexpr int gap = 50;
	
	prev_mid_left = mid_left;
	mid_left = (winSize.w/2)-pad_left;
	content_left = mid_left-100;
	
	if (prev_mid_left != mid_left) {
	    back.x = content_left-50;
	    back.y = 0;
		back.w = box_width;
		back.h = box_height;
		back.updatePair();
	}

	// Set background
	if (state == MAIN_MENU) {
		for (auto &i: button_group) {
			i->setX( (winSize.w/2) + 30 );
            // TODO IMPORTANT dont reference events
			i->update(events, offset_x, offset_y);
		}
	} else if (state == CONFIG_MENU) {
		back_button->setX( (winSize.w/2) + 30 );
		back_button->update(events, offset_x, offset_y);
		for (size_t i = 0; i < c_elements.size(); ++i) {
			c_elements[i]->setX(left);
			c_elements[i]->setY(top+(gap*i));
			c_elements[i]->update(events, offset_x, offset_y);

			c_elements[i]->onChange(update_config_elements);
		}

		if (back_button->clicked) {            
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
	const int MAX_X = (winSize.w + BLOCK_S*2)/BLOCK_S;
	const int MAX_Y = (winSize.h + BLOCK_S*2)/BLOCK_S;
	// Create a grid of tiles as the background
	for (int i = -1; i < MAX_X; ++i) {
		for (int j = -1; j < MAX_Y; ++j) {
		    SDL_Rect block{
				i*BLOCK_S - shift.frame,
				j*BLOCK_S - shift.frame,
				BLOCK_S, BLOCK_S};
			SDL_Rect src{0, 0, 16, 16};

			// Draw the tile
            SDL_RenderCopy(renderer, textures.get_texture(2), &src, &block);
            
		}
	}

	back.render(renderer, textures, offset_x, offset_y);
}

void Menu::set_state(int state) {
	update(false); // If we don't update before we change state it thinks that the button is
	// forever pressed (or the caller of this function in general)
	this->state = state;
	// Update again before we render, or else things may looks strange for a frame
	update(false);
}

void Menu::render_sidebar() {	
	// Draw line
	constexpr int vert_gap = 30;
    SDL_Rect line{offset_x+mid_left+pad_left, offset_y+vert_gap, 2, box_height-(vert_gap*2)};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &line);

	// Lets render a random block and some text
	random_block.render(renderer, textures, offset_x+content_left, offset_y+35, 60, 60);

	header->draw(offset_x+content_left+70, offset_y+45);

	paragraph->draw(offset_x+content_left+5, offset_y+120);
}

int Menu::get_pressed() {
	for (auto &i: button_group) {
		if (i->clicked) {
			return i->id;
		}
	}
	return -1;
}


void Menu::render_main_menu() {
	// Render all buttons
	for (auto &i: button_group) {
		i->render(renderer, textures, offset_x, offset_y);
	}
}

void Menu::render_config_menu() {
	back_button->render(renderer, textures, offset_x, offset_y);
	// Render all elements, no matter the type
	for (auto &i: c_elements) {
		i->render(renderer, textures, offset_x, offset_y);
        
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

