#include "textbox.hpp"

Textbox::Textbox(int id, int x, int y, int width, int height)
	: text{}, id{id}, x{x}, y{y}, width{width}, height{height}, focused{false}, textbox_text{nullptr}, blink{20}
#ifdef __SWITCH__
	, rc{0}, kbd{}
#endif
{
}

Textbox::~Textbox() {}

void Textbox::update(EventWrapper*& events) {
	std::array<int, 2> pos = events->get_vmouse_pos();
	if (events->get_vmouse_clicked()) {
		if (AABB(x, y, width, height, pos[0], pos[1], 1, 1)) {
			focused = true;
#ifdef __SWITCH__
			rc = swkbdCreate(&kbd, 0);
			char tmp[16] = {0};
			if (R_SUCCEEDED(rc)) {
				//swkbdConfigSetTextCheckCallback(&kbd);
				rc = swkbdShow(&kbd, tmp, 15);
				swkbdClose(&kbd);
			}
			
			text = std::string(tmp);
			if (textbox_text != nullptr) {
				textbox_text->set_text(text);
			}
			focused = false;
#else
			blink.reset();
			events->set_text_mode_buffer(text);
			events->enable_text_mode();
#endif
		} else {
			focused = false;
			textbox_text->set_text(text);
			events->disable_text_mode();
			events->clear_text_mode_buffer();
		}
	}
	
	blink.tick();
	if (focused) handle_keyboard(events);
}

void Textbox::handle_keyboard(EventWrapper*& events) {
	bool cursor = false;
	std::string from_buffer = events->get_text_buffer();
	
	text = from_buffer;
	
	if (blink.get_frame() < (blink.get_max_frames()/2)) {
		cursor = true;
	}
	
	
	if (textbox_text != nullptr) {
		textbox_text->set_text(text + (cursor ? "_" : ""));
	}
}

void Textbox::render(SDL_Renderer* renderer) {
	SDL_Rect box;
	box.x = x;
	box.y = y;
	box.w = width;
	box.h = height;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
	SDL_RenderFillRect(renderer, &box);
	
	if (textbox_text == nullptr) {
		SDL_Color color;
		color.r = 255; color.g = 255; color.b = 255; color.a = 255;
		textbox_text = new Text(renderer, text, color, constants::button_font);
	} else {
		textbox_text->draw(x+10, y+((height/2)-20 ));
	}
}
