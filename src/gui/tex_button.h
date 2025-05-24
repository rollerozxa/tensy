#pragma once

#include <SDL3/SDL.h>

typedef struct {
	SDL_FRect rect;
	int texture;
	bool _held;
} TexButton;

#define TEX_BUTTON(obj, rect, tex) \
	obj = (TexButton){rect, tex}

bool tex_button_event(const SDL_Event *ev, TexButton *button);
void tex_button(SDL_Renderer *renderer, TexButton *button);
