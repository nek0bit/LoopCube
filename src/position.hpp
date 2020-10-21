#ifndef POSITION_HPP
#define POSITION_HPP

// Sort of functions similarly to SDL_Rect, except according to the documentation to SDL_Rect, it only supports ints. We would like doubles, please.
struct Position {
	double x, y, w, h;
};


#endif // POSITION_HPP
