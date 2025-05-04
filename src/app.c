#include "app.h"
#include "debug.h"
#include "media/sound.h"
#include "media/textures.h"
#include "overlay.h"
#include "scene.h"
#include "scenes.h"

void AppInit(SDL_Renderer *renderer) {
	sound_init();
	textures_init(renderer);

	register_scenes();

	run_scene_init();
}

void AppEvent(SDL_Event *ev) {
	run_scene_event(ev);
	run_overlay_event(ev);

	debug_event(ev);
}

void AppUpdate(void) {
	run_scene_update();
	run_overlay_update();
}

void AppDraw(SDL_Renderer *renderer) {
	run_scene_draw(renderer);
	run_overlay_draw(renderer);

	debug_draw(renderer);

	perform_scene_transition(renderer);
}

void AppQuit(void) {
	switch_scene("exiting");
}
