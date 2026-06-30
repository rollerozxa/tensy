#include "app.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "gamesettings.h"
#include "media/textures.h"
#include "renderer.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#if !SDL_MIXER_VERSION_ATLEAST(3,2,0) || !SDL_VERSION_ATLEAST(3,4,0)
#error Tensy currently requires being compiled with at least SDL 3.4.0 and SDL_mixer 3.2.0.\
  To build with vendored versions of those libraries pass -DUSE_VENDORED_LIBS=ON to CMake when reconfiguring.

#endif

#include "media/assetloader.h"

SDL_Window *window;
SDL_Renderer *renderer;

#if !EMBEDDED_DATA
bool can_access_data(void) {
	char path[512] = {0};
	snprintf(path, sizeof(path), PREFIX "textures/check.png");
	return access(path, F_OK) == 0;
}
#endif

SDL_AppResult SDL_AppInit(void **rustptr, int argc, char **argv) {
#if !EMBEDDED_DATA && !defined(SDL_PLATFORM_ANDROID)
	const char *exedir = SDL_GetBasePath();
	chdir(exedir);

	// Sanity checks when using loose data files, try to find the data folder
	if (!can_access_data()) {
		chdir("..");
		if (!can_access_data()) {
			chdir("data");
			if (!can_access_data()) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, APP_NAME, "Failed to find game data. Exiting.", NULL);
				return SDL_APP_FAILURE;
			}
		}
	}
#endif

	SDL_srand(0);

	color_load_custom_numbers();
	settings_load();

	SDL_SetAppMetadata(APP_NAME, NULL, APP_ID);

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

	if (argc > 1 && strncmp(argv[1], "-unblur", 7) == 0)
		textures_force_nearest(true);

	SDL_WindowFlags windowflags = 0;
#ifdef WINDOW_RESIZABLE
	windowflags |= SDL_WINDOW_RESIZABLE;
#endif

	SDL_CreateWindowAndRenderer(
		APP_NAME,
		WINDOW_W, WINDOW_H,
		windowflags, &window, &renderer);

	if (!window || !renderer) {
		const char *error = SDL_GetError();
		FMT_STRING(msg, 1024, "Failed to start the game. Error: %s", error);

		SDL_HideWindow(window);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, APP_NAME, msg, NULL);
		return SDL_APP_FAILURE;
	}

	SDL_SetWindowFillDocument(window, true);

#ifdef SDL_PLATFORM_MACOS
	if (strcmp(SDL_GetRendererName(renderer), "opengl") == 0)
		textures_force_nearest(true);
#endif

#ifdef ALWAYS_FULLSCREEN
	if (true)
#else
	if (settings_getflag(FLAG_FULLSCREEN))
#endif
		SDL_SetWindowFullscreen(window, true);

	renderer_set_logical_presentation();

	SDL_SetRenderVSync(renderer, 1);

	AppInit(window, renderer);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *rustptr, SDL_Event *ev) {

	if (ev->type == SDL_EVENT_QUIT
	|| (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_Q
		&& (ev->key.mod & SDL_KMOD_CTRL) && (ev->key.mod & SDL_KMOD_SHIFT))) {
		AppQuit();
		return SDL_APP_CONTINUE;
	}

	if (ev->type == SDL_EVENT_WINDOW_RESIZED) {
		renderer_set_logical_presentation();
		return SDL_APP_CONTINUE;
	}

#if !defined(ALWAYS_FULLSCREEN) && !defined(SDL_PLATFORM_EMSCRIPTEN)
	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_F11 && !ev->key.repeat) {
		settings_toggleflag(FLAG_FULLSCREEN);
		SDL_SetWindowFullscreen(window, settings_getflag(FLAG_FULLSCREEN));
	}
#endif

	SDL_ConvertEventToRenderCoordinates(renderer, ev);

	AppEvent(ev);

	return SDL_APP_CONTINUE;
}

extern bool exiting;

SDL_AppResult SDL_AppIterate(void *rustptr) {
	AppUpdate();

	if (exiting)
		return SDL_APP_SUCCESS;

	AppDraw(renderer);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *rustptr, SDL_AppResult result) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
