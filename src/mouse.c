#include "mouse.h"
#include "draw.h"
#include "virtual_cursor.h"
#include <SDL3/SDL_events.h>

extern SDL_Window *window;

int mouse_get_state_scaled(SDL_FPoint *mouse) {
	int clicked = 0;
	if (virtual_cursor_is_active()) {
		SDL_FPoint vc = virtual_cursor_get_pos();
		mouse->x = vc.x;
		mouse->y = vc.y;
		clicked = virtual_cursor_clicked();
	} else {
		SDL_FPoint temp;
		clicked = SDL_GetMouseState(&temp.x, &temp.y);
		SDL_RenderCoordinatesFromWindow(renderer, temp.x, temp.y, &mouse->x, &mouse->y);
	}

	return clicked;
}

SDL_FPoint mouse_get_point(void) {
	if (virtual_cursor_is_active())
		return virtual_cursor_get_pos();

	SDL_FPoint mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	return mouse;
}
