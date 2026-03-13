#include "color.h"
#include "consts.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "media/music.h"
#include "media/sound.h"
#include "puzzles.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stdio.h>

#define PUZZLE_LEVELS 32
#define PUZZLE_COLS 8

static Button puzzle_buttons[PUZZLE_LEVELS];
static char puzzle_labels[PUZZLE_LEVELS][8];

void puzzle_select_init(void) {
	const float btn_w = 96.0f / 2;
	const float btn_h = 96.0f / 2;

	for (int i = 0; i < puzzle_get_count(); i++) {
		int cx = i % PUZZLE_COLS;
		int cy = i / PUZZLE_COLS;

		float x = 20.f + cx * (btn_w + 10.f);
		float y = 60.f + cy * (btn_h + 10.f);

		snprintf(puzzle_labels[i], sizeof(puzzle_labels[i]), "%d", i + 1);
		SDL_FRect rect = RECT(x, y, btn_w, btn_h);
		BUTTON(puzzle_buttons[i], rect, puzzle_labels[i]);
	}
}

void puzzle_select_event(const SDL_Event *ev) {
	if (is_escaping(ev))
		scene_switch("selectmode");

	for (int i = 0; i < puzzle_get_count(); i++) {
		if (button_event(ev, &puzzle_buttons[i])) {
			music_fade_out(1000);
			// Start game with the selected puzzle
			game.puzzle_id = i;
			scene_switch("game");
		}
	}

	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.key == SDLK_F8) {
		sound_play(SND_WOOZY);
		scene_switch("puzzle_editor");
	}
}

void puzzle_select_draw(void) {
	text_draw_shadow("Select puzzle", 10, 10, 3);

	for (int i = 0; i < puzzle_get_count(); i++)
		button(&puzzle_buttons[i]);
}

Scene puzzle_select_scene = {
	"puzzle_select",
	puzzle_select_init,
	puzzle_select_event,
	NULL,
	puzzle_select_draw,
	CLR_BACKGROUND_1
};
