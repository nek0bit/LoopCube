#include "menu.hpp"

Menu::Menu(SDL_Renderer* renderer,
        TextureHandler &textures,
        EventHandler &events,
        int* WINDOW_W,
        int* WINDOW_H,
		   std::vector<std::string> option_str) : BLOCK_S{40}, BUTTON_W{200}, WINDOW_W{WINDOW_W},
												  WINDOW_H{WINDOW_H}, shift{BLOCK_S}, pad_left{180},
												  header{nullptr} {
    this->renderer = renderer;
    this->textures = &textures;
    this->events = &events;
    
    // Resize option_state to match option_str size
    this->option_str = option_str;
    option_state.resize(option_str.size());
    button_group.resize(option_str.size());
    const int offset_y = 48;

    for (size_t i = 0; i < button_group.size(); ++i) {
        button_group[i] = Button(i, this->renderer, *(this->textures), 
								 0, 30+(offset_y*i), BUTTON_W);
        button_group[i].set_text(option_str[i]);
    }
    
    //checkbox = new Checkbox(0, "text", 20, 100, 40);

	// Setup random block
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, constants::block_info.size()-1);

	int rand_id = constants::block_info.at(dist(rng)).get_id();
	random_block = Item(rand_id, textures, renderer);

	// Setup paragraph string
	for (auto &i: constants::content) {
		p_string += i + "\n";
	}
}

Menu::~Menu() { }

void Menu::update() {
    // Set background
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 255);
	
    auto mouse_pos = events->get_mouse_pos();
    for (auto &i: button_group) {
        i.set_x( (*WINDOW_W/2) + 30 );
        i.update(mouse_pos[0], mouse_pos[1], events->get_mouse_down());
    }

    // Update animation for background
    shift.tick();
    
    //checkbox->update(*events);
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
        if (i.get_pressed()) {
            return i.get_id();
        }
    }
    return -1;
}

void Menu::render() {
    render_background();
	render_sidebar();
    for (auto &i: button_group) {
        i.render();
    }
    
    //checkbox->render(renderer, 0, 0);
}
