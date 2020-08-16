#include "play.hpp"

Play::Play(SDL_Renderer* renderer, TextureHandler &textures, EventHandler &events, int *WINDOW_W, int *WINDOW_H)
    : WINDOW_W{WINDOW_W}, WINDOW_H{WINDOW_H}, camera{WINDOW_W, WINDOW_H}, fade{60} {
    this->renderer = renderer;
    this->textures = &textures;
    this->events = &events;

    camera.set_pos(0, 125);


    unsigned long int seed = 8932478970182;
    // Configure camera
    player = Player(textures);
    chunks = Chunk_Group(seed, textures);
	// TODO keep working here...
    inv = new Inventory(renderer, textures, events, WINDOW_W, WINDOW_H);
}

Play::~Play() {

}

void Play::print_mouse_pos() {
    // Just for debugging
    auto pos = events->get_mouse_pos();
    std::cout << "x: " << pos[0] << " / y: " << pos[1] << " / down: " << events->get_mouse_down() << std::endl;
}

void Play::update() {
    // Update all chunks
    chunks.update_all();
    chunks.check_area(player.get_default_x());

    // Update player
    player.update(chunks);

    inv->update();


    for (int i = 0; i < 4; ++i) {
        if (events->get_state()[i]) {
            player.direct_player(i, chunks);
        }
    }

    // Jump (A)
    if (events->get_button_state()[4]) {
        player.direct_player(0, chunks);
    }

    // Down
    if (events->get_button_state()[0]) {
        player.direct_player(2, chunks);
    }

    // Right
    if (events->get_button_state()[1]) {
        player.direct_player(1, chunks);
    }

    // Left
    if (events->get_button_state()[2]) {
        player.direct_player(3, chunks);
    }

    // Up
    if (events->get_button_state()[3]) {
        player.direct_player(0, chunks);
    }



    


    // Update camera
    handle_camera();

    // Update animation(s)
    fade.tick();
}

void Play::render() {
    chunks.render_all();

    SDL_SetRenderDrawColor(renderer, 0x79, 0xae, 0xd9, 255);
    player.render();



    int p1, p2;
    if (!inv->get_inventory_visibility()) draw_selection(&p1, &p2);

    // Get cursor over chunk
    Chunk* chunk = chunks.get_chunk_at(p1-8);
    if (chunk != nullptr) {
        // Do some math to get the chunk position
        int chunk_pos = std::abs(p1-(chunk->get_slot()*8));
        if (events->get_mouse_down()) {
            chunk->destroy_block(chunk_pos, p2, inv);
        }
    }

    if (!inv->get_inventory_visibility()) inv->draw_hotbar();

    inv->draw_inventory_menu();

    //if (!inv->get_inventory_visibility()) draw_debug_menu();
}

// Draw a selection box and set p1 and p2 to the position
// Perhaps we should rename this to get_selection and seperate rendering functions?
void Play::draw_selection(int* p1, int* p2) {
    int b_w = static_cast<int>(constants::block_w);
    int b_h = static_cast<int>(constants::block_h);

    auto mpos = events->get_mouse_pos();

    const int sel_x = floor((mpos[0] - camera.get_x()) / b_w) * b_w;
    const int sel_y = floor((mpos[1] - camera.get_y()) / b_h) * b_h;

    SDL_Rect selection{sel_x + static_cast<int>(camera.get_x()), sel_y + static_cast<int>(camera.get_y()), b_w, b_h};

    int fade_amount = std::abs(std::sin(static_cast<double>(fade.get_frame())/20))*30+50;


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, fade_amount);
    SDL_RenderFillRect(renderer, &selection);

    *p1 = sel_x/b_w;
    *p2 = sel_y/b_h;
}

// Sets camera to player position
void Play::handle_camera() {
    double x = (player.get_default_x()*-1 + (*WINDOW_W/2)) - player.get_width()/2;
    double y = player.get_default_y()*-1 + (*WINDOW_H/2) - player.get_height()/2;
    camera.set_pos(x, y);
}

/*
void Play::draw_debug_menu() {
    SDL_version compiled;
    SDL_GetVersion(&compiled);

    std::vector<std::string> dbg_text = {
        "[[ DEBUG ]]",
        // Player
        "Built: At "+ std::string(__TIME__)+" "+std::string(__DATE__)+" C++"+std::to_string(__cplusplus)
            +" SDL"+std::to_string(compiled.major)+"."+ std::to_string(compiled.minor)+"."+ std::to_string(compiled.patch)
#ifdef __WIIU__
        +" For Wii U",
#else
        +" for PC",
#endif
        "= Player =",
        "X: " + std::to_string(player.get_default_x() / constants::block_w)
            + " / Y: " + std::to_string(player.get_default_y() / constants::block_h),
        // Chunk
        "= Chunk =",
        "Pos: " + std::to_string(chunks.get_id()),
        "Size: " + std::to_string(chunks.get_chunks()->size())

        
    };
    SDL_Color color;
    // For some reason it must be setup like this...
    color.r = 255; color.g = 255; color.b = 255;
    int size = 14;
    for (size_t i = 0; i < dbg_text.size(); ++i) {
        Text dtext{renderer, dbg_text[i], color, };
        dtext.draw(0,i*size);
    }
}
*/
