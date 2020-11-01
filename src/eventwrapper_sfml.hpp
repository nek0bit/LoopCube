#ifdef INPUT_BACKEND_SFML

#ifndef EVENTWRAPPER_SFML_HPP
#define EVENTWRAPPER_SFML_HPP
#include <iostream>
#include <vector>

#include "backendincludes.hpp"

#include "eventwrapper_internal.hpp"

class EventWrapper_SFML: public EventWrapper {
public:
	EventWrapper_SFML(sf::RenderWindow* screen);
	~EventWrapper_SFML();

	void init() override;
	void update_controllers() override;
	void listen() override;
private:
	sf::RenderWindow* screen;
};

#endif

#endif
