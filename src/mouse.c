#include "mouse.h"

int mouse_get_state_scaled(SDL_Renderer *renderer, float *x, float *y) {
	SDL_FPoint mouse;
	int clicked = SDL_GetMouseState(&mouse.x, &mouse.y);
	SDL_RenderCoordinatesFromWindow(renderer, mouse.x, mouse.y, &mouse.x, &mouse.y);
	*x = mouse.x;
	*y = mouse.y;

	return clicked;
}

SDL_FPoint mouse_get_point(void) {
	SDL_FPoint mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	return mouse;
}
