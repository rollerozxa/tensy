#include "consts.h"
#include "draw.h"
#include "gamesettings.h"
#include "graph.h"
#include "gui/button.h"
#include "input.h"
#include "scene.h"
#include <SDL3/SDL.h>
#include <math.h>
#include <stdint.h>

static Button back_button;

void statistics_numbers_init(void) {
	BUTTON(back_button, RECT(480, 315, 95, 30), "Back");
}

void statistics_numbers_event(const SDL_Event *ev) {
	if (button_event(ev, &back_button) || is_escaping(ev))
		scene_switch("statistics");
}

void statistics_numbers_update(float dt) {

}

static double bgpan = 0;

void statistics_numbers_draw(void) {
	if (settings_getflag(FLAG_REDUCED_MOTION))
		bgpan = 0;
	else
		bgpan = fmod(bgpan + 0.25, 32);

	draw_tiled_bg(bgpan);

	uint32_t *settings_numbers = settings()->numbers;
	graph_draw(settings_numbers);

	button(&back_button);
}

Scene statistics_numbers_scene = {
	"statistics_numbers",
	statistics_numbers_init,
	statistics_numbers_event,
	statistics_numbers_update,
	statistics_numbers_draw,
	0x1F3F8F
};
