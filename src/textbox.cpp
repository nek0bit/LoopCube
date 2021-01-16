#include "textbox.hpp"

Textbox::Textbox(int id, int x, int y, int width, int height)
	: text{}, id{id}, x{x}, y{y}, width{width}, height{height}, focused{false}, textbox_text{nullptr}, blink{20}
#ifdef __SWITCH__
	, rc{0}, kbd{}
#endif
{
}

Textbox::~Textbox() {}

void Textbox::update(EventWrapper& events, int offset_x, int offset_y) {
	if (events.vmouse.clicked) {
		if (Generic::collision<int>(x+offset_x, y+offset_y, width, height, events.vmouse.x, events.vmouse.y, 1, 1)) {
			focused = true;
            // TODO clean this up
#ifdef __SWITCH__
			rc = swkbdCreate(&kbd, 0);
			char tmp[16] = {0};
			if (R_SUCCEEDED(rc)) {
				rc = swkbdShow(&kbd, tmp, 15);
				swkbdClose(&kbd);
			}
			
			text = std::string(tmp);
			if (textbox_text != nullptr) {
				textbox_text->setText(text);
			}
			focused = false;
#else
			blink.frame = 0;
            events.textModeBuffer = text;
            events.textMode = true;
#endif
		} else {
			focused = false;
			textbox_text->setText(text);
            events.textMode = false;
            events.textModeBuffer = "";
		}
	}
	
	blink.tick();
	if (focused) handle_keyboard(events);
}

void Textbox::handle_keyboard(EventWrapper& events) {
	bool cursor = false;
    	
	if (blink.frame < (blink.maxFrames/2)) {
		cursor = true;
	}	
	
	if (textbox_text != nullptr) {
		textbox_text->setText(events.textModeBuffer + (cursor ? "_" : ""));
	}
}

void Textbox::render(SDL_Renderer* renderer, TextureHandler& textures, int offset_x, int offset_y) {
    SDL_Rect box{x+offset_x, y+offset_y, width, height};
	
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &box);
	
	if (textbox_text == nullptr) {
		SDL_Color color{255, 255, 255, 255};
		textbox_text = new Text(renderer, text, color, constants::fontHandler.getFont(0));
	} else {
		textbox_text->draw(x+10, y+((height/2)-20 ));
	}
}
