#include "textures.h"
#include <SDL_QOI/SDL_QOI.h>
#include <stdbool.h>

#include "assetloader.h"

#if EMBEDDED_DATA

unsigned char background_stripes_qoi[] = {
	#embed "../../data/textures/background_stripes.qoi"
};
unsigned int background_stripes_qoi_len = sizeof(background_stripes_qoi);

unsigned char check_qoi[] = {
	#embed "../../data/textures/check.qoi"
};
unsigned int check_qoi_len = sizeof(check_qoi);

unsigned char clock_qoi[] = {
	#embed "../../data/textures/clock.qoi"
};
unsigned int clock_qoi_len = sizeof(clock_qoi);

unsigned char end_qoi[] = {
	#embed "../../data/textures/end.qoi"
};
unsigned int end_qoi_len = sizeof(end_qoi);

unsigned char icon_qoi[] = {
	#embed "../../data/textures/icon.qoi"
};
unsigned int icon_qoi_len = sizeof(icon_qoi);

unsigned char intro_raccoon_qoi[] = {
	#embed "../../data/textures/intro_raccoon.qoi"
};
unsigned int intro_raccoon_qoi_len = sizeof(intro_raccoon_qoi);

unsigned char intro_text_qoi[] = {
	#embed "../../data/textures/intro_text.qoi"
};
unsigned int intro_text_qoi_len = sizeof(intro_text_qoi);

unsigned char pause_qoi[] = {
	#embed "../../data/textures/pause.qoi"
};
unsigned int pause_qoi_len = sizeof(pause_qoi);

unsigned char settings_qoi[] = {
	#embed "../../data/textures/settings.qoi"
};
unsigned int settings_qoi_len = sizeof(settings_qoi);

unsigned char shuffle_qoi[] = {
	#embed "../../data/textures/shuffle.qoi"
};
unsigned int shuffle_qoi_len = sizeof(shuffle_qoi);

unsigned char undo_qoi[] = {
	#embed "../../data/textures/undo.qoi"
};
unsigned int undo_qoi_len = sizeof(undo_qoi);

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
	texture_bank[id] = SDL_CreateTextureFromSurface(renderer, ASSETLOADER_QOI(data, path)); \
	SDL_SetTextureScaleMode(texture_bank[id], textures_get_scalemode())

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
