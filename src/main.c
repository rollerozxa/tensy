#include "app.h"
#include "consts.h"
#include "gamesettings.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdio.h>
#include <unistd.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#if !SDL_MIXER_VERSION_ATLEAST(3,1,0) || !SDL_VERSION_ATLEAST(3,3,0)
#error Tensy currently requires being compiled with a recent enough master version of SDL and SDL_mixer. To build with vendored versions of those libraries pass -DUSE_VENDORED_LIBS=ON to CMake when reconfiguring.
#endif

SDL_Window *window;
SDL_Renderer *renderer;

SDL_AppResult SDL_AppInit(void **rustptr, int argc, char **argv) {
	const char *exedir = SDL_GetBasePath();
	chdir(exedir);

	SDL_srand(0);

	settings_load();

	SDL_SetAppMetadata(APP_NAME, NULL, APP_ID);

	SDL_Init(SDL_INIT_VIDEO);

#ifdef SDL_PLATFORM_WINDOWS
	if (argc > 1 && strncmp(argv[1], "-d3d", 4) == 0)
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11,direct3d");
	else
		// Prefer OpenGL on Windows because direct3d11 is blurry with pixelart scaling,
		// and probably some other reasons.
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	retry_render_creation:
#endif

	SDL_CreateWindowAndRenderer(
		APP_NAME,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0, &window, &renderer);

	if (!window || !renderer) {
		const char *error = SDL_GetError();

#ifdef SDL_PLATFORM_WINDOWS
		// There are many cases where OpenGL just doesn't work on Windows... Usually means
		// there is some more serious issue with your system, but just let the user know and
		// fall back to the direct3d render driver, better than nothing.
		if (strcmp(error, "OpenGL not initialized") == 0) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, APP_NAME, "Failed to start the game using OpenGL. Your graphics card drivers may be broken or out of date.\n\nFalling back to Direct3D rendering. (you can launch the game with '-d3d' to use Direct3D without this message)", NULL);

			SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11,direct3d");

			goto retry_render_creation;
		}
#endif

		FMT_STRING(msg, 1024, "Failed to start the game. Error: %s", SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, APP_NAME, msg, NULL);
		return SDL_APP_FAILURE;
	}

#ifdef ALWAYS_FULLSCREEN
	if (true)
#else
	if (settings_getflag(FLAG_FULLSCREEN))
#endif
		SDL_SetWindowFullscreen(window, true);

	SDL_SetRenderLogicalPresentation(renderer, NATIVE_WIDTH, NATIVE_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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

#ifndef ALWAYS_FULLSCREEN
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
