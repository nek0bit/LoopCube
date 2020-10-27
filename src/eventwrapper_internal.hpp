#ifndef EVENTWRAPPER_INTERNAL_HPP
#define EVENTWRAPPER_INTERNAL_HPP
#include <iostream>
#include <vector>
#include <array>

class EventWrapper {
public:
    EventWrapper();
	virtual ~EventWrapper();

	virtual void update_controllers();
	virtual void resize_input_mappings();
	
	// Dev MUST put all of their event stuff in this function
	virtual void listen();

	// Getters
	std::vector<int> get_key_state();
	std::vector<int> get_key_mapping();
	std::vector<int> get_button_state();
    std::vector<int> get_button_mapping();
	std::array<int, 2> get_vmouse_pos();
	int get_vmouse_down();
	int get_vmouse_clicked();

	// Text mode
	virtual void enable_text_mode();
	virtual void disable_text_mode();
	virtual void set_text_mode_buffer(std::string str);
	virtual void clear_text_mode_buffer();
	virtual std::string& get_text_buffer();

	bool get_quit();
    void enable_quit();
    void disable_quit();
	std::string text_mode_buffer;
protected:
	virtual void init(); // Not often used, but useful for seperating API stuff from main.
	
	bool quit; // If this is true, game will completely quit (useful for windowed graphics engines, if loopcube deals with one)
	           // This option can be forced as well
	std::vector<int> key_mapping; // Maps to proper state index
	std::vector<int> key_state;

	std::vector<int> button_mapping; // Maps to proper state index
	std::vector<int> button_state;

	// vmouse = virtual mouse; Could be a real mouse, could be emulated (via a controller or touch screen)
	int vmouse_x;
	int vmouse_y;
	int vmouse_down; // User is currently pressing vmouse button down
	int vmouse_clicked; // Same as before but only register pressing vmouse once until released
	// Also, vmouse_down/vmouse_clicked must have 0: none; 1: left click; 2: middle click; 3: right click

	/* Dev is responsible for referring to such proper API's; controller, event object
	 *   Examples, referencing SDL2:
	 *    SDL_Joystick* = Prefer variable name "controller" for definitions such as these
	 *    SDL_Event     = Prefer variable name "event" for definitions such as these
	 */
	// Put those here in child class
	
	/* Text mode is what I would define is an object that listens to keypresses
     * and stores it in a variable in ascii/unicode(?) format. You can enable
	 * and disable this variable, with this priveledge, failure to close this 
	 * variable could result in text appearing in other buffers and possibly
	 * a very large variable/memory problems. Closing clears this buffer.
	 */ 
	bool text_mode;
};

#endif
