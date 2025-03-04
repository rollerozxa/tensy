#include "consts.h"
#include "debug.h"
#include "overlay.h"
#include "scene.h"
#include "scenes.h"
#include "sound.h"
#include "textures.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

SDL_Window *window;
SDL_Renderer *renderer;

SDL_AppResult SDL_AppInit(void **rustptr, int argc, char **argv) {
	const char *exedir = SDL_GetBasePath();
	chdir(exedir);

	SDL_srand(0);

	SDL_SetHint(SDL_HINT_VITA_TOUCH_MOUSE_DEVICE, "0");

	SDL_SetAppMetadata("Tensy", NULL, "se.voxelmanip.tensy");

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(
		"Tensy",
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0, &window, &renderer);

	if (!window || !renderer) {
		char msg[1024];
		snprintf(msg, 1023, "Failed to start the game. Error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game", msg, NULL);
	}

#if defined(__ANDROID__) || defined(__vita__)
	SDL_SetWindowFullscreen(window, true);
#endif

	SDL_SetRenderLogicalPresentation(renderer, NATIVE_WIDTH, NATIVE_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_SetRenderVSync(renderer, 1);

	sound_init();
	textures_init(renderer);

	register_scenes();

	run_scene_init();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *rustptr, SDL_Event *ev) {

	if (ev->type == SDL_EVENT_QUIT
	|| (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_Q)) {
		switch_scene("exiting");
		return SDL_APP_CONTINUE;
	}

	static bool is_fullscreen = false;
	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_F11 && !ev->key.repeat) {
		is_fullscreen = !is_fullscreen;
		SDL_SetWindowFullscreen(window, is_fullscreen);
	}

	SDL_ConvertEventToRenderCoordinates(renderer, ev);

	run_scene_event(ev);
	run_overlay_event(ev);

	debug_event(ev);

	return SDL_APP_CONTINUE;
}

extern bool exiting;

SDL_AppResult SDL_AppIterate(void *rustptr) {
	run_scene_update();
	run_overlay_update();

	if (exiting)
		return SDL_APP_SUCCESS;

	run_scene_draw(renderer);
	run_overlay_draw(renderer);

	debug_draw(renderer);

	perform_scene_transition(renderer);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *rustptr, SDL_AppResult result) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
