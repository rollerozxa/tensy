#include <SDL3/SDL.h>
#include <stddef.h>

#include "consts.h"
#include "draw.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "overlay.h"
#include "text.h"

static Button yes_button, no_button;

void endgame_init(void) {
	BUTTON(yes_button, RECT(0, 0, 120, 35), "Yes");
	BUTTON(no_button, RECT(0, 0, 120, 35), "No");
}

void endgame_event(const SDL_Event *ev) {
	if (button_event(ev, &no_button) || is_escaping(ev))
		overlay_hide();

	if (button_event(ev, &yes_button)) {
		overlay_hide();
		gamestate_gameover();
	}
}

void endgame_draw(void) {
	draw_translucent_overlay();

	SDL_FRect pausebg_rect = draw_centered_fill_rect(POINT(20 * 15, 20 * 7.5));

	SDL_FRect text_rect = {pausebg_rect.x, pausebg_rect.y, pausebg_rect.w, 20*2};
	text_draw_shadow_centered("End game?", &text_rect, 2);

	text_rect.y += 40;

	text_draw_shadow_centered("Your current score will be the final result.", &text_rect, 1);

	yes_button.rect.x = pausebg_rect.x + pausebg_rect.w - no_button.rect.w - 10;
	yes_button.rect.y = no_button.rect.y = pausebg_rect.y + 20 * 5;
	button(&yes_button);

	no_button.rect.x = pausebg_rect.x + 10;
	button(&no_button);
}

Overlay endgame_overlay = {
	"endgame",
	endgame_init,
	endgame_event,
	NULL,
	endgame_draw
};
