#include "color.h"
#include "consts.h"
#include "gamemode.h"
#include "gamestate.h"
#include "gui/button.h"
#include "input.h"
#include "media/sound.h"
#include "puzzles.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stdio.h>

// Simple puzzle level grid. Adjust these if you add more puzzle data.
#define PUZZLE_LEVELS 32
#define PUZZLE_COLS 8

static Button puzzle_buttons[PUZZLE_LEVELS];
static char puzzle_labels[PUZZLE_LEVELS][8];

void puzzle_select_init(void) {
	const float btn_w = 96.0f / 2;
	const float btn_h = 96.0f / 2;
	const float start_x = 20.0f;
	const float start_y = 60.0f;
	const float x_spacing = btn_w + 10.0f;
	const float y_spacing = btn_h + 10.0f;

	for (int i = 0; i < puzzle_get_count(); i++) {
		int cx = i % PUZZLE_COLS;
		int cy = i / PUZZLE_COLS;

		float x = start_x + cx * x_spacing;
		float y = start_y + cy * y_spacing;

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
			sound_play(SND_CLICK);
			// Load the selected puzzle into the current game board and start.
			game.puzzle_id = i;
			scene_switch("game");
		}
	}

#ifdef DEBUG
	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.key == SDLK_F8) {
		sound_play(SND_WOOZY);
		scene_switch("puzzle_editor");
	}
#endif
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
