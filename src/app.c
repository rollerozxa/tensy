#include "app.h"
#include "debug.h"
#include "gamemode.h"
#include "gamesettings.h"
#include "media/sound.h"
#include "media/textures.h"
#include "overlay.h"
#include "scene.h"
#include "scenes.h"
#include "toast.h"

void AppInit(SDL_Window *window, SDL_Renderer *renderer) {
	sound_init();
	textures_init(renderer);

	SDL_Surface *icon = get_icon_surface();
	SDL_SetWindowIcon(window, icon);
	SDL_DestroySurface(icon);

	scenes_register();
	init_gamemodes();

	scene_run_init();
}

void AppEvent(SDL_Event *ev) {
	scene_run_event(ev);
	overlay_run_event(ev);

	debug_event(ev);
}

void AppUpdate(void) {
	static uint64_t last_time = 0;
	if (last_time == 0)
		last_time = SDL_GetTicksNS();

	uint64_t now = SDL_GetTicksNS();
	float dt = (now - last_time) / 1e9f;
	last_time = now;

	scene_run_update(dt);
	overlay_run_update();

	update_toast(dt);
}

void AppDraw(SDL_Renderer *renderer) {
	scene_run_draw(renderer);
	overlay_run_draw(renderer);

	draw_toast(renderer);

	debug_draw(renderer);

	scene_perform_transition(renderer);
}

void AppQuit(void) {
	settings_save();
	scene_switch("exiting");
}
