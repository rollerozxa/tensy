#include "consts.h"
#include "draw.h"
#include "gamesettings.h"
#include "gui/button.h"
#include "input.h"
#include "scene.h"
#include "text.h"
#include "timeformat.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static Button leaderboard_button, numbers_button, ok_button;

void statistics_init(void) {
	BUTTON(leaderboard_button, RECT(235,250,170,40), "Leaderboard");

	BUTTON(numbers_button, RECT(20,300,200,40), "Number Statistics");

	BUTTON(ok_button, RECT(245,300,150,40), "Back");
}

void statistics_event(const SDL_Event *ev) {

	if (button_event(ev, &leaderboard_button))
		scene_switch("leaderboard");

	if (button_event(ev, &numbers_button))
		scene_switch("statistics_numbers");

	if (button_event(ev, &ok_button) || is_escaping(ev))
		scene_switch("mainmenu");
}

void statistics_update(float dt) {

}

void statistics_draw(void) {
	text_draw_shadow("Statistics", 10, 10, 3);

	draw_set_color(0x1f3a7e);
	for (int i = 0; i < 3; i++)
		draw_fill_rect(&RECT(20, 60 + i * 50, SCREEN_W - 40, 40));

	{
		char formatted[64], label[96];
		timeformat_duration(settings()->playtime, formatted, sizeof(formatted));
		snprintf(label, sizeof(label), "Total playtime: %s", formatted);

		text_draw_shadow(label, 30, 68, 2);
	}

	{
		char label[96];
		snprintf(label, sizeof(label), "Total numbers removed: %" SDL_PRIu64, settings()->numbers_removed);

		text_draw_shadow(label, 30, 118, 2);
	}

	{
		char label[96];
		snprintf(label, sizeof(label), "Total score accumulated: %" SDL_PRIu64, settings()->total_accumulated_score);

		text_draw_shadow(label, 30, 168, 2);
	}

	button(&leaderboard_button);

	button(&numbers_button);

	button(&ok_button);

}

Scene statistics_scene = {
	"statistics",
	statistics_init,
	statistics_event,
	statistics_update,
	statistics_draw,
	0x1F3F8F
};
