#include "pause.h"
#include "consts.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "render.h"
#include "savestate.h"
#include "scene.h"
#include "text.h"
#include "toast.h"

static Button resume_button, save_button, exit_button;

void pause_init(void) {
	BUTTON(resume_button, RECT(0, 0, 150, 40), "Resume");
	BUTTON(save_button, RECT(0, 0, 150, 40), "Save");
	BUTTON(exit_button, RECT(0, 0, 150, 40), "Exit");
}

void pause_event(const SDL_Event *ev) {
	if (button_event(ev, &resume_button) || is_escaping(ev))
		overlay_hide();

	if (button_event(ev, &save_button)) {
		savestate_save();
		show_toast("Saved!", 2);
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

void pause_draw(SDL_Renderer *renderer) {
	draw_translucent_overlay(renderer);

	SDL_FRect pausebg_rect = draw_centered_fill_rect(renderer, POINT(20 * 10, 20 * 12));

	SDL_FRect text_rect = {pausebg_rect.x, pausebg_rect.y, pausebg_rect.w, 20*2};
	text_draw_shadow_centered(renderer, "Game paused", &text_rect, 2);

	float btn_x = pausebg_rect.x + CENTER(pausebg_rect.w, resume_button.rect.w);

	resume_button.rect.x = btn_x;
	resume_button.rect.y = pausebg_rect.y + 20*3;

	button(renderer, &resume_button);

	save_button.rect.x = btn_x;
	save_button.rect.y = pausebg_rect.y + 20*6;

	button(renderer, &save_button);

	exit_button.rect.x = btn_x;
	exit_button.rect.y = pausebg_rect.y + 20*9;

	button(renderer, &exit_button);
}

Overlay pause_overlay = {
	"pause",
	pause_init,
	pause_event,
	NULL,
	pause_draw
};
