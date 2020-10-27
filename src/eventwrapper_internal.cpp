#include "eventwrapper_internal.hpp"

EventWrapper::EventWrapper() : vmouse_down{0}, vmouse_clicked{0}, quit{false} {}
EventWrapper::~EventWrapper() {}

void EventWrapper::init() {}
void EventWrapper::update_controllers() {}
void EventWrapper::resize_input_mappings() {
	key_state.resize(key_mapping.size());
	button_state.resize(button_mapping.size());
}
void EventWrapper::listen() {}

std::vector<int> EventWrapper::get_key_state() { return key_state; }
std::vector<int> EventWrapper::get_key_mapping() { return key_mapping; }
std::vector<int> EventWrapper::get_button_state() { return button_state; }
std::vector<int> EventWrapper::get_button_mapping() { return button_mapping; }
std::array<int, 2> EventWrapper::get_vmouse_pos() { return std::array<int, 2>{vmouse_x, vmouse_y}; }
int EventWrapper::get_vmouse_down() { return vmouse_down; }
int EventWrapper::get_vmouse_clicked() { return vmouse_clicked; }

void EventWrapper::enable_text_mode() { text_mode = true; }
void EventWrapper::disable_text_mode() { text_mode = false; }
void EventWrapper::set_text_mode_buffer(std::string str) { text_mode_buffer = str; }
void EventWrapper::clear_text_mode_buffer() { set_text_mode_buffer(""); }
std::string& EventWrapper::get_text_buffer() { return text_mode_buffer; }

bool EventWrapper::get_quit() { return quit; }
void EventWrapper::enable_quit() { quit = true; }
void EventWrapper::disable_quit() { quit = false; }
