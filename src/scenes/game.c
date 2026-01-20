#include "board.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "gamemode.h"
#include "gamesettings.h"
#include "gamestate.h"
#include "gui/tex_button.h"
#include "input.h"
#include "media/sound.h"
#include "media/textures.h"
#include "overlay.h"
#include "savestate.h"
#include "scene.h"
#include "text.h"
#include "virtual_cursor.h"
#include <SDL3/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define board game.board
#define time_left game.time_left
#define total_time game.total_time

static bool helddown = false;
static SDL_Point first_held_pos = {-1,-1};
static SDL_Point current_held_pos = {-1,-1};
static int held_sum = -1;

static SDL_Point gp_selector_pos = {0,0};
static bool gp_selecting = false;
static bool gp_selection_enabled = false;

#define GP_REPEAT_INITIAL 0.40f
#define GP_REPEAT_INTERVAL 0.08f

static struct {
	bool pressed[4];
	float repeat_timer;
	bool repeat_started;
} gp_dpad = {0};

static TexButton pause_button, shuffle_button, undo_button, end_button;

static float heartbeat_timer = 0.0f;

static int calculate_sum(void) {
	int sum = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
		if (!board.p[x][y].removed)
			sum += board.p[x][y].number;
	}}

	return sum;
}

static bool do_move(void) {
	int sum = calculate_sum();

	if (sum != 10)
		return false;

	gamestate_checkpoint();

	sound_play(SND_MATCH);

	int removed_cells = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
		board.p[x][y].removed = true;
		removed_cells++;
		game.number_stats[board.p[x][y].number - 1]++;
		settings()->numbers_removed++;
		settings()->numbers[board.p[x][y].number - 1]++;
	}}

	if (board.physics)
		board_physics(&board, game.mode == GM_Infinite);

	int score_addition = sum * (removed_cells-1);
	game.score += score_addition;
	settings()->total_accumulated_score += score_addition;

	return true;
}

// Gamepad D-pad selection

static void gp_dpad_move_selection(int dx, int dy) {
	gp_selector_pos.x = SDL_clamp(gp_selector_pos.x + dx, 0, board.w-1);
	gp_selector_pos.y = SDL_clamp(gp_selector_pos.y + dy, 0, board.h-1);

	if (gp_selecting) {
		current_held_pos = gp_selector_pos;
		held_sum = calculate_sum();
		sound_play(SND_SELECT);
	} else {
		sound_play(SND_SELECT);
	}
}

static void gp_dpad_move_selection_real(bool *pressed) {
	int dx = (pressed[1] ? 1 : 0) - (pressed[0] ? 1 : 0);
	int dy = (pressed[3] ? 1 : 0) - (pressed[2] ? 1 : 0);
	gp_dpad_move_selection(dx, dy);
}

static bool gp_dpad_any_down(bool *pressed) {
	return pressed[0] || pressed[1] || pressed[2] || pressed[3];
}

static void gp_dpad_check_buttons(int b, bool pressed) {
	if (b == SDL_GAMEPAD_BUTTON_DPAD_LEFT)
		gp_dpad.pressed[0] = pressed;
	if (b == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
		gp_dpad.pressed[1] = pressed;
	if (b == SDL_GAMEPAD_BUTTON_DPAD_UP)
		gp_dpad.pressed[2] = pressed;
	if (b == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
		gp_dpad.pressed[3] = pressed;
}

void handle_gp_dpad_update(float dt) {
	if (!gp_selection_enabled)
		return;

	if (!gp_dpad_any_down(gp_dpad.pressed))
		return;

	gp_dpad.repeat_timer -= dt;

	if (!gp_dpad.repeat_started) {
		if (gp_dpad.repeat_timer <= 0.0f) {
			gp_dpad_move_selection_real(gp_dpad.pressed);
			gp_dpad.repeat_started = true;
			gp_dpad.repeat_timer = GP_REPEAT_INTERVAL;
		}
	} else {
		while (gp_dpad.repeat_timer <= 0.0f) {
			gp_dpad_move_selection_real(gp_dpad.pressed);
			gp_dpad.repeat_timer += GP_REPEAT_INTERVAL;
		}
	}
}

// ---

#define BUTTON_SIZE 24
#if defined(SDL_PLATFORM_ANDROID) || defined(SDL_PLATFORM_VITA)
	#define BUTTON_SPACING 12
#else
	#define BUTTON_SPACING 8
#endif

void game_init(void) {
	SDL_FRect button_rect = {
		SCREEN_W - 3 - BUTTON_SIZE,
		3,
		BUTTON_SIZE,
		BUTTON_SIZE
	};

	TEX_BUTTON(pause_button, button_rect, TEX_PAUSE);
	pause_button.tooltip = "Pause";

	button_rect.x -= (BUTTON_SIZE + BUTTON_SPACING);
	TEX_BUTTON(shuffle_button, button_rect, TEX_SHUFFLE);
	shuffle_button.tooltip = "Shuffle";

	button_rect.x -= (BUTTON_SIZE + BUTTON_SPACING);
	TEX_BUTTON(undo_button, button_rect, TEX_UNDO);
	undo_button.tooltip = "Undo";

	button_rect.x -= (BUTTON_SIZE + BUTTON_SPACING);
	TEX_BUTTON(end_button, button_rect, TEX_END);
	end_button.tooltip = "End";

	pause_button.padding = shuffle_button.padding = undo_button.padding = end_button.padding = 3;

	helddown = false;

	first_held_pos = (SDL_Point){-1,-1};
	current_held_pos = (SDL_Point){-1,-1};

	gamestate_clear();

	if (game.loaded_existing) {
		game.loaded_existing = false;
		return;
	}

	game.identifier = SDL_rand_bits() + ((uint64_t)SDL_rand_bits() << 32);

	if (gamemodes[game.mode].time_limit) {
		total_time = time_left = board_sizes[board.boardsize].seconds;
		heartbeat_timer = 0.0f;
	}

	if (current_gamemode().gravity_mode != GRA_Ask)
		board.physics = current_gamemode().gravity_mode == GRA_Always;

	game.shuffles = current_gamemode().unlimited_shuffles ? -1 : 3;

	game.score = 0;
	game.dead = false;
	board_change_size(&board, board.w, board.h, board.scale);
	board_reset(&board);

	gp_selector_pos = (SDL_Point){(int)(board.w / 2), (int)(board.h / 2)};
}

static void begin_move(void) {
	do_move();

	first_held_pos = (SDL_Point){-1,-1};
	current_held_pos = (SDL_Point){-1,-1};

	held_sum = -1;
}

void game_event(const SDL_Event *ev) {
	#define CELL_X (ev->motion.x - board.rect.x) / board.cell_size
	#define CELL_Y (ev->motion.y - board.rect.y) / board.cell_size

	int cx = CELL_X;
	int cy = CELL_Y;

	if (overlay_exists()) return;

	switch (ev->type) {
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (gp_selecting)
				break;

			if (SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &board.rect)) {
				first_held_pos = (struct SDL_Point){cx, cy};
				current_held_pos = first_held_pos;
				helddown = true;
				sound_play(SND_SELECT);
			}
			break;

		case SDL_EVENT_MOUSE_MOTION:
			if (gp_selecting)
				break;

			if (helddown) {
				SDL_Point old_held_pos = {current_held_pos.x, current_held_pos.y};

				current_held_pos.x = CELL_X;
				current_held_pos.y = CELL_Y;

				current_held_pos.x = SDL_clamp(current_held_pos.x, 0, board.w-1);
				current_held_pos.y = SDL_clamp(current_held_pos.y, 0, board.h-1);

				if (current_held_pos.x != old_held_pos.x || current_held_pos.y != old_held_pos.y) {
					sound_play(SND_SELECT);
				}

				held_sum = calculate_sum();
			}
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (gp_selecting)
				break;

			if (helddown) {
				begin_move();
				helddown = false;
			}
			break;

		case SDL_EVENT_KEY_UP:
			if (ev->key.key == SDLK_F5)
				savestate_save();

			if (ev->key.key == SDLK_F6)
				savestate_load();

			if (ev->key.key == SDLK_F1)
				board_shuffle_animated(&board, 1);

			if (ev->key.key == SDLK_U)
				gamestate_undo();

			break;

		case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
			int b = ev->gbutton.button;

			if (b == SDL_GAMEPAD_BUTTON_START) {
				overlay_switch("pause");
				break;
			}

			// D-Pad moves the selector
			if (b == SDL_GAMEPAD_BUTTON_DPAD_LEFT || b == SDL_GAMEPAD_BUTTON_DPAD_RIGHT ||
				b == SDL_GAMEPAD_BUTTON_DPAD_UP || b == SDL_GAMEPAD_BUTTON_DPAD_DOWN) {

				virtual_cursor_disable();
				gp_selection_enabled = true;

				gp_dpad_check_buttons(b, true);
				gp_dpad_move_selection_real(gp_dpad.pressed);

				// mark this D-Pad direction as pressed for repeat handling
				gp_dpad.repeat_started = false;
				gp_dpad.repeat_timer = GP_REPEAT_INITIAL;
				break;
			}

			// SOUTH starts selection when pressed
			if (gp_selection_enabled && b == SDL_GAMEPAD_BUTTON_SOUTH) {
				virtual_cursor_disable();

				first_held_pos = gp_selector_pos;
				current_held_pos = first_held_pos;
				gp_selecting = true;
				held_sum = calculate_sum();
				sound_play(SND_SELECT);
			}
		} break;

		case SDL_EVENT_GAMEPAD_BUTTON_UP: {
			int b = ev->gbutton.button;
			if (gp_selecting && b == SDL_GAMEPAD_BUTTON_SOUTH) {
				begin_move();
				gp_selecting = false;
			}

			gp_dpad_check_buttons(b, false);

			bool any = gp_dpad_any_down(gp_dpad.pressed);
			if (any)
				gp_dpad_move_selection_real(gp_dpad.pressed);

			gp_dpad.repeat_started = false;
			gp_dpad.repeat_timer = any ? GP_REPEAT_INITIAL : 0.0f;
		} break;
	}

	if (tex_button_event(ev, &pause_button) || is_escaping(ev))
		overlay_switch("pause");

	if (game.shuffles != 0) {
		if (tex_button_event(ev, &shuffle_button))
			overlay_switch("shuffle");
	}

	if (gamestate_has_undo()) {
		if (tex_button_event(ev, &undo_button))
			gamestate_undo();
	}

	if (!current_gamemode().time_limit && tex_button_event(ev, &end_button))
		overlay_switch("endgame");
}

void game_update(float dt) {
	board_update(&board, dt);

	if (virtual_cursor_is_active()) {
		gp_selection_enabled = false;
	}

	handle_gp_dpad_update(dt);

	if (current_gamemode().time_limit && !game.dead && (!overlay_exists() || strcmp(overlay_get_current(), "shuffle") == 0)) {
		if (time_left < 0)
			gamestate_gameover();
		else {
			time_left -= dt;

			if (time_left < 10.f) {
				heartbeat_timer += dt;
				if (heartbeat_timer >= 1.0f) {
					sound_play(SND_BEAT);
					heartbeat_timer = 0.0f;
				}
			}
		}
	}
}

void game_draw(void) {
	SDL_FPoint first_held_point = board_to_screen_coord(&board, first_held_pos.x, first_held_pos.y);
	SDL_FPoint current_held_point = board_to_screen_coord(&board, current_held_pos.x, current_held_pos.y);

	SDL_FRect sel_rect = {
		fminf(first_held_point.x, current_held_point.x),
		fminf(first_held_point.y, current_held_point.y),
		fabsf(current_held_point.x - first_held_point.x) + board.cell_size,
		fabsf(current_held_point.y - first_held_point.y) + board.cell_size
	};

	if ((gp_selecting || helddown) && held_sum == 10) {
		draw_set_color(0x00A000);
		draw_fill_rect(&sel_rect);
	}

	board_draw(&board, !settings_getflag(FLAG_MONO_NUMBERS));

	if (helddown || gp_selecting) {
		draw_set_color(0xFFFFFF);
		draw_rect(&sel_rect);
	} else if (gp_selection_enabled) {
		// Draw a dotted one-cell selector at gp_selector_pos
		// TODO: make this better
		SDL_FPoint selp = board_to_screen_coord(&board, gp_selector_pos.x, gp_selector_pos.y);
		SDL_FRect cell_rect = {selp.x, selp.y, board.cell_size, board.cell_size};
		draw_set_color(0xFFFFFF);
		float dot = SDL_min(4.0f, board.cell_size * 0.15f);

		SDL_FRect d = {cell_rect.x + 2, cell_rect.y + 2, dot, dot};
		draw_fill_rect(&d);
		d.x = cell_rect.x + cell_rect.w - 2 - dot;
		draw_fill_rect(&d);
		d.y = cell_rect.y + cell_rect.h - 2 - dot;
		draw_fill_rect(&d);
		d.x = cell_rect.x + 2;
		draw_fill_rect(&d);
	}

	draw_set_color(0x102A6E);
	SDL_FRect bar_rect = {0, 0, SCREEN_W, 30};
	draw_fill_rect(&bar_rect);
	bar_rect.y = SCREEN_H - 30;
	draw_fill_rect(&bar_rect);

	font_set_color(CLR_WHITE);
	if (gamemodes[game.mode].time_limit) {
		int minutes = (time_left+1) / 60;
		int seconds = SDL_max((int)SDL_ceilf(time_left) % 60, 0);

		FMT_STRING(msg, 512, "Score: %d - %d:%02d", game.score, minutes, seconds);
		text_draw(msg, 3,3, 2);
	} else {
		FMT_STRING(msg, 512, "Score: %d", game.score);
		text_draw(msg, 3,3, 2);
	}

	tex_button(&pause_button);

	shuffle_button._disabled = game.shuffles == 0;
	tex_button(&shuffle_button);

	undo_button._disabled = !gamestate_has_undo();
	tex_button(&undo_button);

	if (!current_gamemode().time_limit)
		tex_button(&end_button);

	if (gamemodes[game.mode].time_limit) {
		SDL_FRect progbar_rect = {0, SCREEN_H - 20, -1, 20};
		progbar_rect.w = SCREEN_W * (time_left / total_time);
		if (progbar_rect.w < 40) {
			draw_set_color(0xff0000);
		} else {
			draw_set_color(0x4871da);
		}

		draw_fill_rect(&progbar_rect);
	}
}

Scene game_scene = {
	"game",
	game_init,
	game_event,
	game_update,
	game_draw,
	0x1F3F8F
};
