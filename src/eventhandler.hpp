#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#include <iostream>
#ifdef __SWITCH__
#include <switch.h>
#endif
#include <SDL2/SDL.h>
#include <vector>
#include <array>

// Easier wrapper for working with events in the game
class EventHandler {
public:
    EventHandler();
    ~EventHandler();

    void listen();

    std::vector<int> get_state();
    std::vector<int> get_button_state();
    std::vector<SDL_Scancode> get_keys_set();
    std::array<int, 2> get_mouse_pos();
    int get_mouse_down();
    int get_mouse_clicked();
    SDL_Event get_event();
    void open_controllers();
    
    // Text mode
    void enable_text_mode();
    void disable_text_mode();
    void set_text_mode_buffer(std::string str);
    void clear_text_mode_buffer();
    std::string& get_text_buffer();

    bool get_quit();

private:
    bool quit;
    //const Uint8* keystate;
    /* Keyboard configuration
      [0] = up; default = SDL_SCANCODE_W
      [1] = right; default = SDL_SCANCODE_D
      [2] = down; default = SDL_SCANCODE_S
      [3] = left; default = SDL_SCANCODE_A
      [4] = inventory; default = SDL_SCANCODE_I
      [5...14] = hotbar_slot1...9,0; default = SDL_SCANCODE_1...9,0
      [15] = debug_menu; default = SDL_SCANCODE_F8
	  [16] = create_entity; default = SDL_SCANCODE_C
    */
    std::vector<SDL_Scancode> keys_set;
    std::vector<int> state;


    std::vector<SDL_GameControllerButton> buttons_set;
    std::vector<int> button_state;

    int mouse_x;
    int mouse_y;
    int mouse_down;
    int mouse_clicked;

    #ifdef __SWITCH__
    HidControllerID conID;
    #endif
    
    SDL_Joystick* controller;

    SDL_Event event;
    
    bool text_mode;
    
    std::string text_mode_buffer;
};

#endif // EVENTHANDLER_HPP
