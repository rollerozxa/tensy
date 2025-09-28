#include "scene.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "gamesettings.h"
#include <string.h>

static Scene scenes[MAX_SCENES];
static int current_scene = 0;
static int allocated_scenes = 0;

int scene_add(Scene scene) {
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
static int pending_trans = -1;

void scene_switch_num(int i) {
	trans = true;
	trans_step = 0;
	trans_to = i;
}

static void scene_transition_to(int scene_id) {
	current_scene = scene_id;
	scene_run_init();
}

void scene_switch(const char *name) {
	for (size_t i = 0; i < MAX_SCENES; i++) {
		if (strcmp(name, scenes[i].name) == 0) {
			if (settings_getflag(FLAG_REDUCED_MOTION)) {
				scene_transition_to(i);
				return;
			}

			if (!trans)
				scene_switch_num(i);
			else
				pending_trans = i;

			return;
		}
	}

	SDL_assert(0);
	return;
}

const char *scene_get_current(void) {
	return scenes[current_scene].name;
}

void scene_run_init(void) {
	if (scenes[current_scene].init)
		scenes[current_scene].init();
}

void scene_run_event(const SDL_Event *ev) {
	if (scenes[current_scene].event)
		scenes[current_scene].event(ev);
}

void scene_run_update(float dt) {
	if (scenes[current_scene].update)
		scenes[current_scene].update(dt);
}

void scene_run_draw(void) {
	font_set_color(CLR_WHITE);
	draw_set_color(0x0);
	SDL_RenderClear(renderer);
	draw_set_color(scenes[current_scene].color);
	draw_fill_rect(&FULL_RECT());

	if (scenes[current_scene].draw)
		scenes[current_scene].draw();
}

void scene_perform_transition(void) {
	if (!trans) {
		if (pending_trans != -1) {
			// filter out garbage values caused by spam clicks
			if (current_scene != pending_trans)
				scene_switch_num(pending_trans);

			pending_trans = -1;
		}

		return;
	}

	if (trans_step < 25)
		trans_alpha += 10;
	else
		trans_alpha -= 10;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, trans_alpha);

	draw_set_blend(true);
	draw_fill_rect(&FULL_RECT());

	trans_step++;

	if (trans_step == 25)
		scene_transition_to(trans_to);
	else if (trans_step == 50)
		trans = false;
}

bool scene_is_transitioning(void) {
	return trans;
}
