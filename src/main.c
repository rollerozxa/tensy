#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "consts.h"
#include "game.h"
#include "scene.h"

SDL_Window *window;
SDL_Renderer *renderer;

SDL_AppResult SDL_AppInit(void **rustptr, int argc, char **argv) {
	const char* exedir = SDL_GetBasePath();
	chdir(exedir);

	SDL_srand(0);

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

	SDL_SetRenderLogicalPresentation(renderer, NATIVE_WIDTH, NATIVE_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_SetRenderVSync(renderer, 1);

	add_scene((Scene){"game", game_init, game_event, game_update, game_draw});

	SDL_assert(switch_scene("game") == 1);

	run_scene_init();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *rustptr, SDL_Event *ev) {

	if (ev->type == SDL_EVENT_QUIT || (ev->type == SDL_EVENT_KEY_DOWN && ev->key.key == SDL_SCANCODE_Q)) {
		return SDL_APP_SUCCESS;
	}

	SDL_ConvertEventToRenderCoordinates(renderer, ev);

	run_scene_event(ev);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *rustptr) {
	run_scene_update();

	run_scene_draw(renderer);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *rustptr, SDL_AppResult result) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
