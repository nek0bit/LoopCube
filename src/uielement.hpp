#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP

#include <iostream>
#include "eventhandler.hpp"

class UiElement {
public:
  UiElement();
  virtual ~UiElement();

  virtual void update();
  virtual void update(EventHandler& events);
  virtual void render();
  virtual void render(SDL_Renderer* renderer);
  virtual void render(SDL_Renderer* renderer, int x_offset, int y_offset);
};

#endif
