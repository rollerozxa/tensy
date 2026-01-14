#include "textures.h"
#include <SDL_QOI/SDL_QOI.h>
#include <stdbool.h>

#include "assetloader.h"

#if EMBEDDED_DATA
#include <data/background_stripes_png.h>
#include <data/check_png.h>
#include <data/clock_png.h>
#include <data/cursor_png.h>
#include <data/end_png.h>
#include <data/icon_png.h>
#include <data/infinite_png.h>
#include <data/intro_raccoon_png.h>
#include <data/intro_text_png.h>
#include <data/itchio_png.h>
#include <data/pause_png.h>
#include <data/settings_png.h>
#include <data/shuffle_png.h>
#include <data/undo_png.h>
#endif

static SDL_Texture *texture_bank[100];
static bool textures_loaded = false;

static bool force_nearest = false;

void textures_force_nearest(bool force) {
	force_nearest = force;
}

SDL_ScaleMode textures_get_scalemode(void) {
	return force_nearest ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_PIXELART;
}

#define LOAD_TEX(id, data, path) \
	texture_bank[id] = SDL_CreateTextureFromSurface(renderer, ASSETLOADER_PNG(data, path)); \
	SDL_SetTextureScaleMode(texture_bank[id], textures_get_scalemode())

#define LOAD_TEX_LIN(id, data, path) \
	texture_bank[id] = SDL_CreateTextureFromSurface(renderer, ASSETLOADER_PNG(data, path)); \
	SDL_SetTextureScaleMode(texture_bank[id], SDL_SCALEMODE_LINEAR)

void textures_init(SDL_Renderer *renderer) {
	LOAD_TEX(TEX_BG_STRIPES, background_stripes_png, "background_stripes.png");
	LOAD_TEX(TEX_CHECK, check_png, "check.png");
	LOAD_TEX(TEX_CLOCK, clock_png, "clock.png");
	LOAD_TEX(TEX_CURSOR, cursor_png, "cursor.png");
	LOAD_TEX(TEX_END, end_png, "end.png");
	LOAD_TEX(TEX_INFINITE, infinite_png, "infinite.png");
	LOAD_TEX(TEX_INTRO_RACCOON, intro_raccoon_png, "intro_raccoon.png");
	LOAD_TEX_LIN(TEX_INTRO_TEXT, intro_text_png, "intro_text.png");
	LOAD_TEX(TEX_ITCHIO, itchio_png, "itchio.png");
	LOAD_TEX(TEX_PAUSE, pause_png, "pause.png");
	LOAD_TEX(TEX_SETTINGS, settings_png, "settings.png");
	LOAD_TEX(TEX_SHUFFLE, shuffle_png, "shuffle.png");
	LOAD_TEX(TEX_UNDO, undo_png, "undo.png");

	textures_loaded = true;
}

SDL_Surface *get_icon_surface(void) {
	return ASSETLOADER_PNG(icon_png, "icon.png");
}

SDL_Texture *textures_get(int texture_id) {
	return texture_bank[texture_id];
}
