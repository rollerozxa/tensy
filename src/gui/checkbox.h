#pragma once

#include <SDL3/SDL.h>

typedef struct {
	SDL_FPoint *pos;
	bool checked;
	const char *label;
} Checkbox;

bool checkbox_event(const SDL_Event *ev, Checkbox *checkbox);
bool checkbox(SDL_Renderer *renderer, Checkbox *checkbox);
