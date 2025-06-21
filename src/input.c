#include "input.h"

bool is_escaping(const SDL_Event *ev) {
	return ev->type == SDL_EVENT_KEY_UP
		&& (ev->key.key == SDLK_ESCAPE || ev->key.key == SDLK_AC_BACK);
}
