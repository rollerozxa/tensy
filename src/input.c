#include "input.h"
#include "gamepad.h"

bool is_escaping(const SDL_Event *ev) {
	return (ev->type == SDL_EVENT_KEY_UP
		&& (ev->key.key == SDLK_ESCAPE || ev->key.key == SDLK_AC_BACK))
		|| (gamepad_enabled() && ev->type == SDL_EVENT_GAMEPAD_BUTTON_UP && ev->gbutton.button == SDL_GAMEPAD_BUTTON_EAST);
}
