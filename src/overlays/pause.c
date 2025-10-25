#include "consts.h"
#include "draw.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "savestate.h"
#include "scene.h"
#include "text.h"
#include "toast.h"
#include <SDL3/SDL.h>
#include <stddef.h>

static Button resume_button, statistics_button, save_button, exit_button;

void pause_init(void) {
	BUTTON(resume_button, RECT(0, 0, 150, 40), "Resume");
	BUTTON(statistics_button, RECT(0, 0, 150, 40), "Statistics");
	BUTTON(save_button, RECT(0, 0, 150, 40), "Save");
	BUTTON(exit_button, RECT(0, 0, 150, 40), "Exit");
}

void pause_event(const SDL_Event *ev) {
	if (button_event(ev, &resume_button) || is_escaping(ev))
		overlay_hide();

	if (button_event(ev, &statistics_button))
		overlay_switch("game_statistics_numbers");

	if (button_event(ev, &save_button)) {
		savestate_save();
		toast_show("Saved!", 2);
	}

	if (button_event(ev, &exit_button)) {
		if (game.dirty)
			overlay_switch("exitconfirm");
		else {
			overlay_hide();
			scene_switch("mainmenu");
		}
	}
}

void pause_draw(void) {
	draw_translucent_overlay();

	SDL_FRect pausebg_rect = draw_centered_fill_rect(POINT(20 * 10, 20 * 14), 0x102a63);

	SDL_FRect text_rect = {pausebg_rect.x, pausebg_rect.y, pausebg_rect.w, 20*2};
	text_draw_shadow_centered("Game paused", &text_rect, 2);

	float btn_x = pausebg_rect.x + CENTER(pausebg_rect.w, resume_button.rect.w);

	Button *buttons[] = {&resume_button, &statistics_button, &save_button, &exit_button};

	button_bar(*buttons, btn_x, pausebg_rect.y + 20*2.5, 20);
}

Overlay pause_overlay = {
	"pause",
	pause_init,
	pause_event,
	NULL,
	pause_draw
};
