#include "game.h"
#include "board.h"
#include "consts.h"
#include "colours.h"
#include "font.h"
#include "gui/button.h"
#include "mouse.h"
#include "overlay.h"
#include "render.h"
#include "settings.h"
#include "scene.h"
#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static bool helddown = false;
static SDL_Point first_held_pos = {-1,-1};
static SDL_Point current_held_pos = {-1,-1};
static int held_sum = -1;

static int score = 0;

static Button pause_button;

static int calculate_sum(void) {
	int sum = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
		if (!board.p[x][y].removed)
			sum += board.p[x][y].number;
	}}

	return sum;
}

static void board_physics(void) {
	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (int y = board.h - 1; y >= 0; y--) {
		if (!board.p[x][y].removed)
			continue;

		int src_y = y - 1;
		while (src_y >= 0 && board.p[x][src_y].removed)
			src_y--;

		if (src_y >= 0) {
			board.p[x][y].number = board.p[x][src_y].number;
			board.p[x][y].removed = false;
			board.p[x][src_y].removed = true;
		} else {
			board.p[x][y].number = 0;
			board.p[x][y].removed = true;
		}
	}}
}

static void do_move(void) {
	int sum = calculate_sum();

	if (sum != 10)
		return;

	sound_play(SND_MATCH);

	int removed_cells = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
		board.p[x][y].removed = true;
		removed_cells++;
	}}

	if (settings()->board_physics)
		board_physics();

	score += sum * (removed_cells-1);
}

void game_init(void) {
	BUTTON(pause_button, RECT(NATIVE_WIDTH-100,0,100,25), "Pause");

	helddown = false;

	first_held_pos = (SDL_Point){-1,-1};
	current_held_pos = (SDL_Point){-1,-1};

	board_change_size(board.w, board.h, 2);
	board_reset();
}

void game_event(const SDL_Event *ev) {
	#define CELL_X (ev->motion.x - board.rect.x) / board.cell_size
	#define CELL_Y (ev->motion.y - board.rect.y) / board.cell_size

	if (has_overlay()) return;

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int cx = CELL_X;
		int cy = CELL_Y;

		if (SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &board.rect)) {
			first_held_pos = (struct SDL_Point){cx, cy};
			current_held_pos = first_held_pos;
			helddown = true;
			sound_play(SND_SELECT);
		}
	} else if (ev->type == SDL_EVENT_MOUSE_MOTION) {
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
	} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		do_move();

		first_held_pos = (SDL_Point){-1,-1};
		current_held_pos = (SDL_Point){-1,-1};

		held_sum = -1;
		helddown = false;
	} else if (ev->type == SDL_EVENT_KEY_UP) {
		if (ev->key.key == SDLK_AC_BACK)
			switch_scene("mainmenu");
	}

	if (button_event(ev, &pause_button))
		switch_overlay("pause");
}

void game_update(void) {

}

void game_draw(SDL_Renderer *renderer) {
	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});

	SDL_Point first_held_point = board_to_screen_coord(first_held_pos.x,first_held_pos.y);
	SDL_Point current_held_point = board_to_screen_coord(current_held_pos.x, current_held_pos.y);

	SDL_FRect sel_rect = {
		fminf(first_held_point.x, current_held_point.x),
		fminf(first_held_point.y, current_held_point.y),
		abs(current_held_point.x - first_held_point.x) + board.cell_size,
		abs(current_held_point.y - first_held_point.y) + board.cell_size
	};

	if (helddown) {
		if (held_sum == 10) {
			set_draw_color(renderer, 0x00A000);

			SDL_RenderFillRect(renderer, &sel_rect);
		}
	}

	board_draw(renderer, settings()->coloured_numbers);

	if (helddown) {
		set_draw_color(renderer, 0xFFFFFF);
		SDL_RenderRect(renderer, &sel_rect);
	}

	set_draw_color(renderer, 0x102A6E);
	SDL_FRect bar_rect = {0, 0, NATIVE_WIDTH, 20};
	SDL_RenderFillRect(renderer, &bar_rect);
	bar_rect.y = NATIVE_HEIGHT - 20;
	SDL_RenderFillRect(renderer, &bar_rect);

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});
	char msg[512];
	snprintf(msg, 511, "Score: %d", score);
	draw_text(renderer, msg, 0,0, 2);

	button(renderer, &pause_button);
}

Scene game_scene = {
	"game",
	game_init,
	game_event,
	game_update,
	game_draw,
	0x1F3F8F
};
