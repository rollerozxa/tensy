#include "mouse.h"
#include "draw.h"

int mouse_get_state_scaled(SDL_FPoint *mouse) {
	SDL_FPoint temp;
	int clicked = SDL_GetMouseState(&temp.x, &temp.y);
	SDL_RenderCoordinatesFromWindow(renderer, temp.x, temp.y, &mouse->x, &mouse->y);

	return clicked;
}

SDL_FPoint mouse_get_point(void) {
	SDL_FPoint mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	return mouse;
}
