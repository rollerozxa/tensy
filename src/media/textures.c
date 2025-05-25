#include "textures.h"

#include <data/background_stripes.h>
#include <data/check.h>
#include <data/clock.h>
#include <data/pause.h>
#include <data/shuffle.h>

#include <SDL_QOI/SDL_QOI.h>

static SDL_Texture *texture_bank[100];
static bool textures_loaded = false;

#define LOAD_TEX(id, data) \
	texture_bank[id] = SDL_CreateTextureFromSurface(renderer, SDL_LoadQOI_IO(SDL_IOFromMem(data, data##_len))); \
	SDL_SetTextureScaleMode(texture_bank[id], SDL_SCALEMODE_NEAREST)

void textures_init(SDL_Renderer *renderer) {
	LOAD_TEX(TEX_BG_STRIPES, background_stripes_qoi);
	LOAD_TEX(TEX_CHECK, check_qoi);
	LOAD_TEX(TEX_CLOCK, clock_qoi);
	LOAD_TEX(TEX_PAUSE, pause_qoi);
	LOAD_TEX(TEX_SHUFFLE, shuffle_qoi);

	textures_loaded = true;
}

SDL_Texture *textures_get(int texture_id) {
	return texture_bank[texture_id];
}
