#include "app.h"
#include "debug.h"
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

	register_scenes();

	run_scene_init();
}

void AppEvent(SDL_Event *ev) {
	run_scene_event(ev);
	run_overlay_event(ev);

	debug_event(ev);
}

void AppUpdate(void) {
	static uint64_t last_time = 0;
	if (last_time == 0)
		last_time = SDL_GetTicksNS();

	uint64_t now = SDL_GetTicksNS();
	float dt = (now - last_time) / 1e9f;
	last_time = now;

	run_scene_update(dt);
	run_overlay_update();

	update_toast(dt);
}

void AppDraw(SDL_Renderer *renderer) {
	run_scene_draw(renderer);
	run_overlay_draw(renderer);

	draw_toast(renderer);

	debug_draw(renderer);

	perform_scene_transition(renderer);
}

void AppQuit(void) {
	switch_scene("exiting");
}
