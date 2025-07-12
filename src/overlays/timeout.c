#include "timeout.h"
#include "consts.h"
#include "font.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "render.h"
#include "scene.h"
#include <stdio.h>

static Button exit_button;

void timeout_init(void) {
	BUTTON(exit_button, RECT(0, 0, 150, 40), "Return");
}

void timeout_event(const SDL_Event *ev) {
	if (button_event(ev, &exit_button) || is_escaping(ev)) {
		hide_overlay();
		switch_scene("selectmode");
	}
}

void timeout_draw(SDL_Renderer *renderer) {
	draw_translucent_overlay(renderer);

	SDL_FRect bg_rect = draw_centered_fill_rect(renderer, POINT(20 * 10, 20 * 12));

	SDL_FRect text_rect = {bg_rect.x, bg_rect.y, bg_rect.w, 20*2};
	draw_text_shadow_centered(renderer, "Out of time!", &text_rect, 2);

	SDL_FPoint score_pos = { bg_rect.x + 20, bg_rect.y + 20*3 };

	draw_text_shadow(renderer, "Final score:", score_pos.x, score_pos.y, 1.5);
	char scoretext[128];
	snprintf(scoretext, 127, "%d", game.score);
	score_pos.y += 25;
	draw_text_shadow(renderer, scoretext, score_pos.x, score_pos.y, 1.5);

	exit_button.rect.x = bg_rect.x + CENTER(bg_rect.w, exit_button.rect.w);
	exit_button.rect.y = bg_rect.y + 20*9;

	button(renderer, &exit_button);
}

Overlay timeout_overlay = {
	"timeout",
	timeout_init,
	timeout_event,
	NULL,
	timeout_draw
};
