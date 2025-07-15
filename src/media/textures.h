#pragma once

#include <SDL3/SDL.h>

enum {
	TEX_BG_STRIPES,
	TEX_CHECK,
	TEX_CLOCK,
	TEX_INTRO_RACCOON,
	TEX_INTRO_TEXT,
	TEX_PAUSE,
	TEX_SHUFFLE,
	TEX_UNDO
};

void textures_init(SDL_Renderer *renderer);

SDL_Texture *textures_get(int texture_id);

SDL_Surface *get_icon_surface(void);
