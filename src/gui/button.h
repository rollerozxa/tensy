#pragma once

#include <SDL3/SDL.h>

typedef struct {
	SDL_FRect rect;
	const char *text;
	bool _held;
} Button;

#define BUTTON(obj, rect, text) \
	obj = (Button){rect, text}

bool button_event(const SDL_Event *ev, Button *button);
void button(SDL_Renderer *renderer, Button *button);
