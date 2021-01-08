#include "inventory.hpp"

Inventory::Inventory(SDL_Renderer* renderer,
                     TextureHandler& textures,
					 EventWrapper& events,
                     WinSize& winSize)
    : winSize{winSize},
      animation{false},
      hotbar_slots{10},
      max_slots{hotbar_slots*5},
      visible{true},
      show_inventory_menu{false},
      hotbar_pos{0},
      item_held{},
      renderer{renderer},
      textures{textures},
      events{events} {	
	items.resize(max_slots, Item{});

	inventory_slide = Transition{static_cast<double>(winSize.h), 0.30};
	hotbar_slide = Transition{0, 0.30};
}

Inventory::~Inventory() {}

Item& Inventory::get_selected_item() {
	return items[hotbar_pos];
}

void Inventory::add_item(int id) {
	constexpr int max_count = 99;

	auto found = std::find_if(items.begin(), items.end(), [&](Item item) {
		if (!item.enabled) return false; 
		return item.get_block().get_id() == id ? item.get_count() < max_count : false;
	});

	if (found != items.end()) {
		found->add_count();
	} else {
		for (Item& i: items) {
			if (!i.enabled) {
				i = Item{renderer, id};
				break;
			}
		}
	}
}

void Inventory::update() {
	// hotbar_slot keys
	for (int i = 5; i < 15; ++i) {
		if (events.key_state[i]) {
			hotbar_pos = i-5;
		}
	}

	if (events.key_state[4] || events.button_state[8]) {
		show_inventory_menu = !show_inventory_menu;
		animation = show_inventory_menu;
	}

	inventory_slide.update();
	hotbar_slide.update();
}

void Inventory::draw_inventory_menu() {
	// Sliding Animation
	if (animation) {
		inventory_slide.value = 0;
	} else {
	    inventory_slide.value = winSize.h;
	}

	int slide = inventory_slide.get();

	constexpr int offset = 1;
	if (slide < winSize.h - offset) {
		int scale = 3; // TODO Fix with hovered position

		constexpr int menu_width = 170;
		constexpr int menu_height = 90;
		
		const int MAX_X = (winSize.w - (menu_width*scale))/2;
		const int MAX_Y = (winSize.h - (menu_height*scale))/2;

		// First draw BG shadow
		SDL_Rect shadow{0, static_cast<int>(slide), winSize.w, winSize.h};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &shadow);

		// Render inventory menu
		SDL_Rect src{0, 0, menu_width, menu_height};
		SDL_Rect dest{MAX_X, MAX_Y+static_cast<int>(slide), src.w*scale, src.h*scale};
        SDL_RenderCopy(renderer, textures.get_texture(11), &src, &dest);

		std::vector<int> pos = get_hovered_pos(events.vmouse.x, events.vmouse.y, MAX_X, MAX_Y+slide, true);
		
		if (events.vmouse.clicked == 1 && pos[0] != -1 && pos[1] != -1) {
			auto& it = items[pos[0]+(pos[1]*hotbar_slots)];
			if (!item_held.enabled) {
				// move item; null out inventory slot
				item_held = it;
				it = Item{};
			} else {
				// Swap items
				auto tmp = it;
				it = item_held;
				item_held = tmp;
			}
		}
		
		if (item_held.enabled) item_held.render(renderer, textures, events.vmouse.x-17, events.vmouse.y-17, 35, 35);
	}
}

// returns std::vector<int>{-1, -1} if nothing is being hovered
std::vector<int> Inventory::get_hovered_pos(int x, int y, int corner_x, int corner_y, bool draw = false) {
	constexpr int tile_size = 42;

	// Start gap
	constexpr int sx_gap = 18;
	constexpr int sy_gap = 18;

	// Splitter gap
	constexpr int sp_gap = 6;
	int total = max_slots/10;

	std::vector<int> returner;
	
	// Loop through slots
	for (int i = 0; i < hotbar_slots; ++i) {
		for (int j = 0; j < total; ++j) {
			// Check collision

			int new_x = corner_x+(i*(tile_size+sp_gap)+sx_gap);
			int new_y = corner_y+(j*(tile_size+sp_gap)+sy_gap);

			// Draw item
			if (items[i+(j*hotbar_slots)].enabled) {
				items[i+(j*hotbar_slots)].render(renderer, textures, new_x+3, new_y+3, tile_size-7, tile_size-7);
			}

			if (collision(x, y, 1, 1,
						  new_x, new_y, tile_size, tile_size)) {

				if (draw) {
				    SDL_Rect rect{new_x, new_y, tile_size, tile_size};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 40);
                    SDL_RenderFillRect(renderer, &rect);
				}

				returner = {i, j};
			}
		}
	}

	if (returner.size() == 2) {
		return returner;
	}

	return std::vector<int>{-1, -1};
}

// TODO this is a duplicate of AABB function :(
bool Inventory::collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	SDL_Rect r1{x1, y1, w1, h1}, r2{x2, y2, w2, h2};

	if (r1.x < r2.x + r2.w &&
		r1.x + r1.w > r2.x &&
		r1.y < r2.y + r2.h &&
		r1.y + r1.h > r2.y) {
		return true;
	}
	return false;
}

bool Inventory::get_inventory_visibility() {
	return show_inventory_menu;
}

void Inventory::draw_hotbar() {
	constexpr int BLOCK_S = 40;
    const int MAX_X = (winSize.w - ((hotbar_slots+1)*BLOCK_S+3))/2;
	constexpr int offset = -1;
	
	// Animation
	if (animation) {
		hotbar_slide.value = (BLOCK_S*-1) + offset;
	} else {
		hotbar_slide.value = 0;
	}

	int slide = hotbar_slide.get();
	
	for (int i = 0; i < hotbar_slots; ++i) {
	    SDL_Rect src{0, 0, 16, 16};
		if (hotbar_pos == i) {
			src.y = 16;
		}
	    SDL_Rect block{i*(BLOCK_S+3)+MAX_X, slide, BLOCK_S, BLOCK_S};
        SDL_RenderCopy(renderer, textures.get_texture(10), &src, &block);

		if (items[i].enabled) {
			items[i].render(renderer, textures, block.x+5, slide+block.y+5, 30, 30);
		}
	}
}
