#include "timeout.h"
#include "consts.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "draw.h"
#include "scene.h"
#include "text.h"
#include "highscores.h"
#include <stdio.h>

static Button exit_button;

void timeout_init(void) {
	BUTTON(exit_button, RECT(0, 0, 150, 40), "Return");

	highscore_register(game);
}

void timeout_event(const SDL_Event *ev) {
	if (button_event(ev, &exit_button) || is_escaping(ev)) {
		overlay_hide();
		scene_switch("selectmode");
	}
}

void timeout_draw(void) {
	draw_translucent_overlay();

	SDL_FRect bg_rect = draw_centered_fill_rect(POINT(20 * 10, 20 * 12));

	SDL_FRect text_rect = {bg_rect.x, bg_rect.y, bg_rect.w, 20*2};
	text_draw_shadow_centered("Out of time!", &text_rect, 2);

	SDL_FPoint score_pos = { bg_rect.x + 20, bg_rect.y + 20*3 };

	text_draw_shadow("Final score:", score_pos.x, score_pos.y, 1.5);
	FMT_STRING(scoretext, 128, "%d", game.score);
	score_pos.y += 25;
	text_draw_shadow(scoretext, score_pos.x, score_pos.y, 1.5);

	exit_button.rect.x = bg_rect.x + CENTER(bg_rect.w, exit_button.rect.w);
	exit_button.rect.y = bg_rect.y + 20*9;

	button(&exit_button);
}

Overlay timeout_overlay = {
	"timeout",
	timeout_init,
	timeout_event,
	NULL,
	timeout_draw
};
