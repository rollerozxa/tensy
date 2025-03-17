#pragma once

#include <SDL3/SDL.h>

typedef struct {
	SDL_FRect rect;
	int min;
	int max;
	int value;
	int _pos;
	bool _dragging;
	int _handle_width;
} Slider;

#define SLIDER(obj, rect, min, max, value) \
	obj = (Slider){(rect), min, max, value}; \
	slider_init(&obj)

void slider_init(Slider *slider);
bool slider_event(const SDL_Event *ev, Slider *slider);
void slider(SDL_Renderer *renderer, Slider *slider);
