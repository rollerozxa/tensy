#include "renderer.h"
#include "consts.h"
#include "gamesettings.h"
#include <SDL3/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;

bool window_is_scaled_down(void) {
	SDL_Point res = {0};
	SDL_GetWindowSizeInPixels(window, &res.x, &res.y);

	return res.x < WINDOW_W || res.y < WINDOW_H;
}

#ifndef SUPPORTS_PIXEL_PERFECT
#define SUPPORTS_PIXEL_PERFECT 0
#endif

void renderer_set_logical_presentation(void) {
#ifdef SDL_PLATFORM_VITA

	SDL_SetRenderLogicalPresentation(renderer, SCREEN_W, SCREEN_H+4, SDL_LOGICAL_PRESENTATION_STRETCH);
#else
	bool integer_scale = SUPPORTS_PIXEL_PERFECT && settings_getflag(FLAG_PIXEL_PERFECT) && !window_is_scaled_down();

	int presentation_mode = integer_scale ? SDL_LOGICAL_PRESENTATION_INTEGER_SCALE : SDL_LOGICAL_PRESENTATION_LETTERBOX;
	SDL_SetRenderLogicalPresentation(renderer, SCREEN_W, SCREEN_H, presentation_mode);
#endif
}
