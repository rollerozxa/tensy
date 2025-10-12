#include "textures.h"
#include <SDL_QOI/SDL_QOI.h>
#include <stdbool.h>

#include "assetloader.h"

#if EMBEDDED_DATA
#include <data/background_stripes_qoi.h>
#include <data/check_qoi.h>
#include <data/clock_qoi.h>
#include <data/end_qoi.h>
#include <data/icon_qoi.h>
#include <data/intro_raccoon_qoi.h>
#include <data/intro_text_qoi.h>
#include <data/pause_qoi.h>
#include <data/settings_qoi.h>
#include <data/shuffle_qoi.h>
#include <data/undo_qoi.h>
#endif

static SDL_Texture *texture_bank[100];
static bool textures_loaded = false;

#define LOAD_TEX(id, data, path) \
	texture_bank[id] = SDL_CreateTextureFromSurface(renderer, ASSETLOADER_QOI(data, path)); \
	SDL_SetTextureScaleMode(texture_bank[id], SDL_SCALEMODE_PIXELART)

#define LOAD_TEX_LIN(id, data, path) \
	texture_bank[id] = SDL_CreateTextureFromSurface(renderer, ASSETLOADER_QOI(data, path)); \
	SDL_SetTextureScaleMode(texture_bank[id], SDL_SCALEMODE_LINEAR)

void textures_init(SDL_Renderer *renderer) {
	LOAD_TEX(TEX_BG_STRIPES, background_stripes_qoi, "background_stripes.qoi");
	LOAD_TEX(TEX_CHECK, check_qoi, "check.qoi");
	LOAD_TEX(TEX_CLOCK, clock_qoi, "clock.qoi");
	LOAD_TEX(TEX_END, end_qoi, "end.qoi");
	LOAD_TEX(TEX_INTRO_RACCOON, intro_raccoon_qoi, "intro_raccoon.qoi");
	LOAD_TEX_LIN(TEX_INTRO_TEXT, intro_text_qoi, "intro_text.qoi");
	LOAD_TEX(TEX_PAUSE, pause_qoi, "pause.qoi");
	LOAD_TEX(TEX_SETTINGS, settings_qoi, "settings.qoi");
	LOAD_TEX(TEX_SHUFFLE, shuffle_qoi, "shuffle.qoi");
	LOAD_TEX(TEX_UNDO, undo_qoi, "undo.qoi");

	textures_loaded = true;
}

SDL_Surface *get_icon_surface(void) {
	return ASSETLOADER_QOI(icon_qoi, "icon.qoi");
}

SDL_Texture *textures_get(int texture_id) {
	return texture_bank[texture_id];
}
