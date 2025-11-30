#include "consts.h"
#include "draw.h"
#include "gamestate.h"
#include "graph.h"
#include "overlay.h"
#include "text.h"

static Button back_button;

void game_statistics_numbers_init(void) {
	BUTTON(back_button, RECT(480, 315, 95, 30), "Back");
}

void game_statistics_numbers_event(const SDL_Event *ev) {
	if (button_event(ev, &back_button))
		overlay_switch("pause");
}

void game_statistics_numbers_draw(void) {
	draw_translucent_overlay();

	SDL_FRect rect = {
		(float)SCREEN_W / 2 - (SCREEN_W * 0.8f) / 2,
		0,
		SCREEN_W * 0.8f,
		45
	};
	draw_set_color(0x071a45);
	draw_fill_rect(&rect);

	text_draw_shadow("Per-number game statistics", 80, 20, 2);

	uint32_t *settings_numbers = game.number_stats;
	graph_draw(settings_numbers);

	button(&back_button);
}

Overlay game_statistics_numbers_overlay = {
	"game_statistics_numbers",
	game_statistics_numbers_init,
	game_statistics_numbers_event,
	NULL,
	game_statistics_numbers_draw
};
