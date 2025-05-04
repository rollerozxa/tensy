#pragma once

#include <SDL3/SDL.h>

enum {
	TEX_BG_STRIPES,
	TEX_CHECK
};

void textures_init(SDL_Renderer *renderer);

SDL_Texture *textures_get(int texture_id);
