#include "shuffle.h"
#include "board.h"
#include "consts.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "draw.h"
#include "text.h"
#include <stdio.h>

static Button yes_button, no_button;

void shuffle_init(void) {
	BUTTON(yes_button, RECT(0, 0, 70, 25), "Yes");
	BUTTON(no_button, RECT(0, 0, 70, 25), "No");
}

void shuffle_event(const SDL_Event *ev) {
	if (button_event(ev, &yes_button)) {
		gamestate_clear();
		board_shuffle_animated(&game.board, 1);
		game.shuffles--;
		overlay_hide();
	}

	if (button_event(ev, &no_button) || is_escaping(ev))
		overlay_hide();
}

void shuffle_draw(void) {
	SDL_FRect bg_rect = {23 * 20, 24, 9 * 20, 20 * 6};
	draw_set_color(0x102a63);
	draw_fill_rect(&bg_rect);

	SDL_FRect text_rect = {bg_rect.x, bg_rect.y, bg_rect.w, 20*2};
	text_draw_shadow_centered("Shuffle?", &text_rect, 2);

	SDL_FPoint score_pos = { bg_rect.x + 10, bg_rect.y + 20*2 };

	text_draw_shadow("Shuffles left:", score_pos.x, score_pos.y, 1.5);
	char scoretext[128];
	snprintf(scoretext, 127, "%d", game.shuffles);
	score_pos.x += 130;
	text_draw_shadow(scoretext, score_pos.x, score_pos.y, 1.5);

	float btn_x = bg_rect.x + CENTER(bg_rect.w, yes_button.rect.w);

	yes_button.rect.x = bg_rect.x + 10;
	yes_button.rect.y = bg_rect.y + 20 * 4;

	button(&yes_button);

	no_button.rect.x = bg_rect.x + 100;
	no_button.rect.y = bg_rect.y + 20 * 4;

	button(&no_button);
}

Overlay shuffle_overlay = {
	"shuffle",
	shuffle_init,
	shuffle_event,
	NULL,
	shuffle_draw
};
