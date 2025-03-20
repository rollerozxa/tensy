#include "scene.h"
#include "consts.h"
#include "render.h"

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

static bool trans = false;
static int trans_step = 0;
static int trans_to = -1;
static int trans_alpha = 0;

int switch_scene(const char *name) {
	if (trans)
		return 2;

	for (size_t i = 0; i < MAX_SCENES; i++) {
		if (strcmp(name, scenes[i].name) == 0) {
			trans = true;
			trans_step = 0;
			trans_to = i;
			return 1;
		}
	}

	SDL_assert(0);
	return 0;
}

const char *get_current_scene(void) {
	return scenes[current_scene].name;
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
	set_draw_color(renderer, scenes[current_scene].colour);
	SDL_RenderClear(renderer);

	if (scenes[current_scene].draw)
		scenes[current_scene].draw(renderer);
}

void perform_scene_transition(SDL_Renderer *renderer) {
	if (!trans) return;

	if (trans_step < 25)
		trans_alpha += 10;
	else
		trans_alpha -= 10;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, trans_alpha);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(renderer, &RECT(0,0,SCREEN_WIDTH,SCREEN_HEIGHT));

	trans_step++;

	if (trans_step == 25) {
		current_scene = trans_to;

		run_scene_init();
	} else if (trans_step == 50)
		trans = false;
}
