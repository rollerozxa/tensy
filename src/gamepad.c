#include "gamepad.h"
#include "consts.h"
#include "toast.h"
#include <SDL3/SDL.h>

static SDL_Gamepad *_gamepad = NULL;

static inline SDL_JoystickID getControllerInstanceID(SDL_Gamepad *controller) {
	return SDL_GetJoystickID(
			SDL_GetGamepadJoystick(controller));
}

void gamepad_init(void) {
	_gamepad = gamepad_find();
}

SDL_Gamepad *gamepad_find(void) {
	int num_gamepads;
	SDL_JoystickID *gamepads = SDL_GetGamepads(&num_gamepads);
	for (int i = 0; i < num_gamepads; i++)
		return SDL_OpenGamepad(gamepads[i]);

	return NULL;
}

void gamepad_handle_hotswap(const SDL_Event *ev) {
	switch (ev->type) {
		case SDL_EVENT_GAMEPAD_ADDED:
			if (!_gamepad) {
				_gamepad = SDL_OpenGamepad(ev->gdevice.which);
				FMT_STRING(msg, 512, "Gamepad %s connected", SDL_GetGamepadName(_gamepad));
				toast_show(msg, 2.0f);
			}
			break;

		case SDL_EVENT_GAMEPAD_REMOVED:
			if (_gamepad && ev->gdevice.which == getControllerInstanceID(_gamepad)) {
				FMT_STRING(msg, 512, "Gamepad %s disconnected", SDL_GetGamepadName(_gamepad));
				toast_show(msg, 2.0f);
				SDL_CloseGamepad(_gamepad);
				_gamepad = gamepad_find();
			}
			break;
	}
}

SDL_Gamepad *gamepad_get() {
	return _gamepad;
}

SDL_FPoint gamepad_get_left_stick(void) {
	SDL_FPoint pos = {0.0f, 0.0f};
	if (!_gamepad)
		return pos;

	pos.x = (float) SDL_GetGamepadAxis(_gamepad,
		SDL_GAMEPAD_AXIS_LEFTX) / (float) INT16_MAX;
	pos.y = (float) SDL_GetGamepadAxis(_gamepad,
		SDL_GAMEPAD_AXIS_LEFTY) / (float) INT16_MAX;

	return pos;
}
