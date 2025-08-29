#include "gameconfig.h"
#include "board.h"
#include "color.h"
#include "consts.h"
#include "font.h"
#include "gamesettings.h"
#include "gamestate.h"
#include "gui/button.h"
#include "gui/checkbox.h"
#include "input.h"
#include "media/music.h"
#include "scene.h"
#include "text.h"
#include <stdio.h>

static bool show_hyuge = false;

static Board board_preview = {NULL, 30, 15, 2};

static Button go_button;
static Checkbox physics_checkbox;

void gameconfig_init(void) {

	for (size_t i = 0; i < num_board_sizes; i++) {
		int btn_width = 100;
		SDL_FRect button_rect = {150 + i * (btn_width + 15), 75, btn_width, 40};

		BUTTON(board_sizes[i].button, button_rect, board_sizes[i].name);
	}

	BUTTON(go_button, RECT(0,NATIVE_HEIGHT-40, NATIVE_WIDTH, 40), "Play");

	CHECKBOX(physics_checkbox, POINT(20, 140), false, "Board physics");

	board_preview.rect_offset = POINT(150, 50);
	board_change_size(&board_preview, board_sizes[2].w, board_sizes[2].h, board_sizes[2].scale -1);
	board_zerofill(&board_preview);
	game.board.boardsize = 2;
}

void gameconfig_event(const SDL_Event *ev) {

	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_LSHIFT)
		show_hyuge = true;
	else if (ev->type == SDL_EVENT_KEY_UP && ev->key.scancode == SDL_SCANCODE_LSHIFT)
		show_hyuge = false;

	if (is_escaping(ev))
		scene_switch("selectmode");

	for (size_t i = 0; i < num_board_sizes; i++) {
		BoardSize *board_size = &board_sizes[i];
		if ((i != 3 || show_hyuge) && button_event(ev, &board_size->button)) {
			game.board.boardsize = i;
			board_change_size(&game.board, board_size->w, board_size->h, board_size->scale);
			board_change_size(&board_preview, board_size->w, board_size->h, SDL_max(0.5f, board_size->scale - 1));
			board_zerofill(&game.board);
			board_zerofill(&board_preview);
		}
	}

	if (button_event(ev, &go_button)) {
		music_fade_out(1000);
		scene_switch("game");
	}

	if (current_gamemode().gravity_mode == GRA_Ask && checkbox_event(ev, &physics_checkbox))
		game.board.physics = physics_checkbox.checked;
}

void gameconfig_draw(void) {
	text_draw_shadow("Configure game", 10, 10, 3);

	board_draw(&board_preview, !settings_getflag(FLAG_MONO_NUMBERS));

	SDL_FRect label_rect = RECTCPY(board_sizes[0].button.rect);
	label_rect.x = 25;
	text_draw_shadow_centered("Board size", &label_rect, 2);
	for (size_t i = 0; i < num_board_sizes; i++) {
		if (i == 3 && !show_hyuge)
			continue;

		button(&board_sizes[i].button);
	}

	if (current_gamemode().gravity_mode == GRA_Ask)
		checkbox(&physics_checkbox);
	else if (current_gamemode().gravity_mode == GRA_Always)
		text_draw_shadow("Board physics enabled", physics_checkbox.pos.x, physics_checkbox.pos.y, 2);

	if (gamemodes[game.mode].time_limit) {
		font_set_color(CLR_WHITE);

		int duration = board_sizes[game.board.boardsize].seconds;
		int minutes = (duration+1) / 60;
		int seconds = SDL_max((int)SDL_ceilf(duration) % 60, 0);

		FMT_STRING(msg, 512, "Time: %d:%02d", minutes, seconds);
		text_draw_shadow(msg, 20, 180, 2);
	} else {
		text_draw_shadow("No time limit", 20, 180, 2);
	}


	button(&go_button);
}

Scene gameconfig_scene = {
	"gameconfig",
	gameconfig_init,
	gameconfig_event,
	NULL,
	gameconfig_draw,
	0x1F3F8F
};
