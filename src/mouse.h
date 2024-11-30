#pragma once

#include <SDL3/SDL.h>

int mouse_get_state_scaled(SDL_Renderer *renderer, float *x, float *y);
SDL_FPoint mouse_get_point(void);
