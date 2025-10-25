#include "consts.h"
#include "draw.h"
#include "gamestate.h"
#include "graph.h"
#include "overlay.h"

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
