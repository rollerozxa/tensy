#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
	SDL_FRect rect;
	const char **items;
	int item_count;
	int selected;
	bool _held;
	bool _open;
	int _hover_index;
} Dropdown;

#define DROPDOWN(obj, rect, items, count, selected) \
	obj = (Dropdown){rect, items, count, selected, false, false, -1}

bool dropdown_event(const SDL_Event *ev, Dropdown *dropdown);
void dropdown(Dropdown *dropdown);
