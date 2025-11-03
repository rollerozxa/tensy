#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
	SDL_FRect rect;
	int texture;
	float padding;
	const char *tooltip;
	bool _held;
	bool _disabled;
	SDL_FRect _calc_rect; // Calculated full rect based on padding
} TexButton;

#define TEX_BUTTON(obj, rect, tex) \
	obj = (TexButton){rect, tex, 0, NULL}

bool tex_button_event(const SDL_Event *ev, TexButton *button);
void tex_button(TexButton *button);
