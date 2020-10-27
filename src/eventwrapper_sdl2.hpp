#include "eventwrapper_internal.hpp"

#ifdef INPUT_BACKEND_SDL2
#include "backendincludes.hpp"

#ifndef EVENTWRAPPER_SDL2_HPP
#define EVENTWRAPPER_SDL2_HPP
#include <iostream>
#include <vector>
#include <array>

class EventWrapper_SDL2: public EventWrapper {
public:
	EventWrapper_SDL2();
	~EventWrapper_SDL2();

	virtual void init() override;
	virtual void update_controllers() override;

	virtual void listen() override;
private:
	SDL_Joystick* controller;
}

#endif // EVENTWRAPPER_SDL2_HPP
#endif // INPUT_BACKEND_SDL2
