#include "leaderboard.h"
#include "consts.h"
#include "draw.h"
#include "gui/dropdown.h"
#include "highscores.h"
#include "input.h"
#include "scene.h"
#include "gamesettings.h"
#include "text.h"

static Dropdown gamemode_dropdown;
static const char *gamemode_dropdown_options[] = {
	"Classic",
	"Gravity",
	"Leisure",
	"Lucky",
	"Five"
};

void leaderboard_init(void) {
	int num_options = sizeof(gamemode_dropdown_options) / sizeof(gamemode_dropdown_options[0]);

	if (!settings_getflag(FLAG_SECRET_FIVE))
		num_options--;

	DROPDOWN(gamemode_dropdown, RECT(340, 15, 180, 30), gamemode_dropdown_options, num_options, 0);
}

void leaderboard_event(const SDL_Event *ev) {

	if (dropdown_event(ev, &gamemode_dropdown)) {

	}

	if (is_escaping(ev))
		scene_switch("mainmenu");
}

void leaderboard_update(float dt) {

}

void leaderboard_draw(void) {
	text_draw_shadow("Leaderboard", 10, 10, 3);

	for (int i = 0; i < MAX_HIGHSCORES; i++) {
		SDL_FRect rect = {20, 60 + 30 * i, 400, 40};

		if (rect.y > NATIVE_HEIGHT)
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

		FMT_STRING(hs_score, 16, "%lu", highscores()[i].score);
		text_draw_shadow(hs_score, row.x + 40, row.y, 2);
	}

	dropdown(&gamemode_dropdown);
}

Scene leaderboard_scene = {
	"leaderboard",
	leaderboard_init,
	leaderboard_event,
	leaderboard_update,
	leaderboard_draw,
	0x1F3F8F
};
