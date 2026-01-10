#pragma once

#include <SDL3/SDL.h>

SDL_Gamepad *gamepad_find(void);
void gamepad_init(void);
void gamepad_handle_hotswap(const SDL_Event *ev);

SDL_Gamepad *gamepad_get(void);
static inline bool gamepad_is_connected(void) { return gamepad_get() != NULL; }

SDL_FPoint gamepad_get_left_stick(void);
