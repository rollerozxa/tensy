#include "scene.h"
#include "consts.h"

static Scene scenes[MAX_SCENES];
static int current_scene = 0;
static int allocated_scenes = 0;

int add_scene(Scene scene) {
	if (allocated_scenes > MAX_SCENES) {
		SDL_assert(0);
		return 0;
	}

	scenes[allocated_scenes] = scene;
	allocated_scenes++;

	return 1;
}

int switch_scene(const char* name) {
	for (size_t i = 0; i < MAX_SCENES; i++) {
		if (name == scenes[i].name) {
			current_scene = i;
			run_scene_init();
			return 1;
		}
	}

	SDL_assert(0);
	return 0;
}

void run_scene_init(void) {
	if (scenes[current_scene].init)
		scenes[current_scene].init();
}

void run_scene_event(const SDL_Event *ev) {
	if (scenes[current_scene].event)
		scenes[current_scene].event(ev);
}

void run_scene_update(void) {
	if (scenes[current_scene].update)
		scenes[current_scene].update();
}

void run_scene_draw(SDL_Renderer *renderer) {
	if (scenes[current_scene].draw)
		scenes[current_scene].draw(renderer);
}
