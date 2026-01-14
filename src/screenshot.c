#include "screenshot.h"
#include "SDL3/SDL_filesystem.h"
#include "consts.h"
#include "fileio.h"
#include <SDL3/SDL.h>

extern SDL_Renderer *renderer;

static bool screenshot_requested = false;

void screenshot_queue(void) {
	screenshot_requested = true;
}

void screenshot_poll(void) {
	if (screenshot_requested) {
		screenshot();
		screenshot_requested = false;
	}
}

static char pref_path[512];

void screenshot(void) {
	SDL_DateTime now;
	SDL_Time ticks;
	SDL_GetCurrentTime(&ticks);
	SDL_TimeToDateTime(ticks, &now, true);

	if (!pref_path[0]) {
		fileio_pref_path(pref_path, sizeof(pref_path), "screenshots");
		SDL_CreateDirectory(pref_path);
	}

	FMT_STRING(path, 512, "%s/screenshot_%04d%02d%02d_%02d%02d%02d.png",
		pref_path,
		now.year, now.month, now.day,
		now.hour, now.minute, now.second);

	int w, h;
	SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
	SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);

	if (!SDL_SavePNG(surface, path))
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to save screenshot to %s: %s", path, SDL_GetError());
	else
		SDL_Log("Screenshot saved to %s", path);
}
