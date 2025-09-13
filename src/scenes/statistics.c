#include <SDL3/SDL.h>

#include "consts.h"
#include "gui/button.h"
#include "input.h"
#include "scene.h"
#include "text.h"

static Button ok_button;

void statistics_init(void) {
	BUTTON(ok_button, RECT(245,300,150,40), "Ok");

}

void statistics_event(const SDL_Event *ev) {

	if (ev->type == SDL_EVENT_KEY_UP) {
		if (ev->key.key == SDLK_1)
			scene_switch("leaderboard");
	}

	if (button_event(ev, &ok_button) || is_escaping(ev))
		scene_switch("mainmenu");
}

void statistics_update(float dt) {

}

void statistics_draw(void) {
	text_draw_shadow("Statistics", 10, 10, 3);

	text_draw_shadow("(coming soon...)", 30, 60, 2);

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
