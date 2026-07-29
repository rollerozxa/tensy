#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
	SDL_FPoint pos;
	const char *text;
	bool _held;
} Hyperlink;

#define HYPERLINK(obj, pos, text) \
	obj = (Hyperlink){pos, text}

bool hyperlink_event(const SDL_Event *ev, Hyperlink *link);
void hyperlink(Hyperlink *link);
