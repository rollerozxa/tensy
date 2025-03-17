#pragma once

#include <SDL3/SDL.h>

typedef struct {
	SDL_FPoint pos;
	bool checked;
	const char *label;
} Checkbox;

#define CHECKBOX(obj, pos, checked, label) \
	obj = (Checkbox){pos, checked, label}

bool checkbox_event(const SDL_Event *ev, Checkbox *checkbox);
bool checkbox(SDL_Renderer *renderer, Checkbox *checkbox);
