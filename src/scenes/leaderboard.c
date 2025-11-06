#include "consts.h"
#include "draw.h"
#include "gamesettings.h"
#include "gui/dropdown.h"
#include "highscores.h"
#include "input.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>

static Dropdown gamemode_dropdown, board_size_dropdown;
static Button back_button;

void leaderboard_init(void) {
	int num_options = sizeof(gamemode_names) / sizeof(gamemode_names[0]);

	if (!settings_getflag(FLAG_SECRET_FIVE))
		num_options--;

	DROPDOWN(gamemode_dropdown, RECT(230, 15, 150, 30), gamemode_names, num_options, 0);
	DROPDOWN(board_size_dropdown, RECT(400, 15, 210, 30), board_size_options, 4, 0);

	BUTTON(back_button, RECT(450,300,150,40), "Back");
}

void leaderboard_event(const SDL_Event *ev) {

	dropdown_event(ev, &gamemode_dropdown);
	dropdown_event(ev, &board_size_dropdown);

	if (is_escaping(ev) || button_event(ev, &back_button))
		scene_switch("statistics");
}

void leaderboard_update(float dt) {

}

void leaderboard_draw(void) {
	text_draw_shadow("Leaderboard", 10, 10, 3);

	Highscore *leaderboard = highscores[gamemode_dropdown.selected][board_size_dropdown.selected];
	for (int i = 0; i < MAX_HIGHSCORES; i++) {
		SDL_FRect rect = {20, 60 + 30 * i, 400, 40};

		if (rect.y > SCREEN_H)
			continue;

		draw_set_color(i % 2 == 0 ? 0x1f3a7e : 0x21449b);
		draw_fill_rect(&rect);

		SDL_FPoint row = { rect.x + 20, rect.y + 5 };

		//char text[16];
		//snprintf(text, 511, "%d. %lu", i+1, highscores()[i].score);
		if ((i+1) > 9)
			row.x -= 12;

		FMT_STRING(hs_rank, 4, "%d", i+1);
		text_draw_shadow(hs_rank, row.x, row.y, 2);

		if ((i+1) > 9)
			row.x += 12;

		text_draw_shadow(".", row.x + 12, row.y, 2);

		FMT_STRING(hs_name, 20, "%s", leaderboard[i].name);
		text_draw_shadow(hs_name, row.x + 40, row.y, 2);

		FMT_STRING(hs_score, 16, "%d", leaderboard[i].score);
		text_draw_shadow(hs_score, row.x + 200, row.y, 2);
	}

	dropdown(&gamemode_dropdown);
	dropdown(&board_size_dropdown);

	button(&back_button);
}

Scene leaderboard_scene = {
	"leaderboard",
	leaderboard_init,
	leaderboard_event,
	leaderboard_update,
	leaderboard_draw,
	0x1F3F8F
};
