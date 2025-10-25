#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
	SDL_FRect rect;
	const char *text;
	bool _held;
	bool _disabled;
} Button;

#define BUTTON(obj, rect, text) \
	obj = (Button){rect, text}

bool button_event(const SDL_Event *ev, Button *button);
void button(Button *button);

void button_bar(Button *buttons, float x, float y, float yp);
