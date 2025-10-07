#include "consts.h"
#include "gui/button.h"
#include "input.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>

static Button leaderboard_button, ok_button;

void statistics_init(void) {
	BUTTON(leaderboard_button, RECT(235,250,170,40), "Leaderboard");

	BUTTON(ok_button, RECT(245,300,150,40), "Back");
}

void statistics_event(const SDL_Event *ev) {

	if (button_event(ev, &leaderboard_button))
		scene_switch("leaderboard");

	if (button_event(ev, &ok_button) || is_escaping(ev))
		scene_switch("mainmenu");
}

void statistics_update(float dt) {

}

void statistics_draw(void) {
	text_draw_shadow("Statistics", 10, 10, 3);

	text_draw_shadow("(coming soon...)", 30, 60, 2);

	button(&leaderboard_button);

	button(&ok_button);

}

Scene statistics_scene = {
	"statistics",
	statistics_init,
	statistics_event,
	statistics_update,
	statistics_draw,
	0x1F3F8F
};
