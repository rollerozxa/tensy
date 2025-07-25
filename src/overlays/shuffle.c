#include "shuffle.h"
#include "board.h"
#include "consts.h"
#include "font.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "render.h"
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
		hide_overlay();
	}

	if (button_event(ev, &no_button) || is_escaping(ev))
		hide_overlay();
}

void shuffle_draw(SDL_Renderer *renderer) {
	SDL_FRect bg_rect = {23 * 20, 24, 9 * 20, 20 * 6};
	set_draw_color(renderer, 0x102a63);
	SDL_RenderFillRect(renderer, &bg_rect);

	SDL_FRect text_rect = {bg_rect.x, bg_rect.y, bg_rect.w, 20*2};
	draw_text_shadow_centered(renderer, "Shuffle?", &text_rect, 2);

	SDL_FPoint score_pos = { bg_rect.x + 10, bg_rect.y + 20*2 };

	draw_text_shadow(renderer, "Shuffles left:", score_pos.x, score_pos.y, 1.5);
	char scoretext[128];
	snprintf(scoretext, 127, "%d", game.shuffles);
	score_pos.x += 130;
	draw_text_shadow(renderer, scoretext, score_pos.x, score_pos.y, 1.5);

	float btn_x = bg_rect.x + CENTER(bg_rect.w, yes_button.rect.w);

	yes_button.rect.x = bg_rect.x + 10;
	yes_button.rect.y = bg_rect.y + 20 * 4;

	button(renderer, &yes_button);

	no_button.rect.x = bg_rect.x + 100;
	no_button.rect.y = bg_rect.y + 20 * 4;

	button(renderer, &no_button);
}

Overlay shuffle_overlay = {
	"shuffle",
	shuffle_init,
	shuffle_event,
	NULL,
	shuffle_draw
};
