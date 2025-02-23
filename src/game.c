#include "game.h"
#include "consts.h"
#include "colours.h"
#include "font.h"
#include "gui/button.h"
#include "overlay.h"
#include "settings.h"
#include "scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int number;
	bool removed;
} Cell;

typedef struct {
	int x;
	int y;
} Neighbour;

#define BOARD_W 30
#define BOARD_H 15

#define BOARD_SCALE 2
#define CELL_SIZE (BOARD_SCALE * 10)
#define FULL_BOARD_WIDTH (BOARD_W * CELL_SIZE)
#define FULL_BOARD_HEIGHT (BOARD_H * CELL_SIZE)

static Cell board[BOARD_W][BOARD_H];

static bool helddown = false;
static SDL_Point first_held_pos = {-1,-1};
static SDL_Point current_held_pos = {-1,-1};
static int held_sum = -1;

static int score = 0;

static Button pause_button = {RECT(NATIVE_WIDTH-100,0,100,25), "Pause"};

static int calculate_sum(void) {
	int sum = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
		if (!board[x][y].removed)
			sum += board[x][y].number;
	}}

	return sum;
}

static void board_physics(void) {
	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (int y = BOARD_H - 1; y >= 0; y--) {
		if (!board[x][y].removed)
			continue;

		int src_y = y - 1;
		while (src_y >= 0 && board[x][src_y].removed)
			src_y--;

		if (src_y >= 0) {
			board[x][y].number = board[x][src_y].number;
			board[x][y].removed = false;
			board[x][src_y].removed = true;
		} else {
			board[x][y].number = 0;
			board[x][y].removed = true;
		}
	}}
}

static void do_move(void) {
	int sum = calculate_sum();

	if (sum != 10)
		return;

	int removed_cells = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
		board[x][y].removed = true;
		removed_cells++;
	}}

	if (settings()->board_physics)
		board_physics();

	score += sum * (removed_cells-1);
}

static SDL_Point board_to_screen_coord(int x, int y) {
	SDL_Point point = {
		(NATIVE_WIDTH-FULL_BOARD_WIDTH) / 2,
		(NATIVE_HEIGHT-FULL_BOARD_HEIGHT) / 2
	};
	point.x += x * CELL_SIZE;
	point.y += y * CELL_SIZE;

	return point;
}

void game_init(void) {
	for (size_t x = 0; x < BOARD_W; x++) {
	for (size_t y = 0; y < BOARD_H; y++) {
		board[x][y].number = SDL_rand(9) + 1;
		board[x][y].removed = false;
	}}
}

void game_event(const SDL_Event *ev) {
	#define CELL_X (ev->motion.x - (float)(NATIVE_WIDTH-FULL_BOARD_WIDTH) / 2) / CELL_SIZE
	#define CELL_Y (ev->motion.y - (float)(NATIVE_HEIGHT-FULL_BOARD_HEIGHT) / 2) / CELL_SIZE

	if (has_overlay()) return;

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int cx = CELL_X;
		int cy = CELL_Y;

		if (SDL_clamp(cx, 0, BOARD_W-1) == cx && SDL_clamp(cy, 0, BOARD_H-1) == cy) {
			first_held_pos = (struct SDL_Point){cx,cy};
			current_held_pos = first_held_pos;
			helddown = true;
		}
	} else if (ev->type == SDL_EVENT_MOUSE_MOTION) {
		if (helddown) {
			current_held_pos.x = CELL_X;
			current_held_pos.y = CELL_Y;

			current_held_pos.x = SDL_clamp(current_held_pos.x, 0, BOARD_W-1);
			current_held_pos.y = SDL_clamp(current_held_pos.y, 0, BOARD_H-1);

			held_sum = calculate_sum();
		}
	} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		do_move();

		first_held_pos = (struct SDL_Point){-1,-1};
		current_held_pos = (struct SDL_Point){-1,-1};

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
	SDL_SetRenderDrawColor(renderer, 0x1f, 0x3f, 0x8f, 0xFF);

	SDL_RenderClear(renderer);

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});

	SDL_Point first_held_point = board_to_screen_coord(first_held_pos.x,first_held_pos.y);
	SDL_Point current_held_point = board_to_screen_coord(current_held_pos.x, current_held_pos.y);

	SDL_FRect sel_rect = {
		fminf(first_held_point.x, current_held_point.x),
		fminf(first_held_point.y, current_held_point.y),
		abs(current_held_point.x - first_held_point.x) + CELL_SIZE,
		abs(current_held_point.y - first_held_point.y) + CELL_SIZE
	};

	if (first_held_pos.x != -1) {
		if (held_sum == 10) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0xA0, 0x00, 0xFF);

			SDL_RenderFillRect(renderer, &sel_rect);
		}
	}

	for (size_t x = 0; x < BOARD_W; x++) {
	for (size_t y = 0; y < BOARD_H; y++) {
		if (settings()->coloured_numbers)
			set_font_color(num_to_colour(board[x][y].number));

		SDL_Point point = board_to_screen_coord(x,y);
		point.x += 3;
		point.y -= 1;

		if (!board[x][y].removed)
			draw_char_shadow(renderer, board[x][y].number + '0',
				point.x, point.y, BOARD_SCALE);
	}}

	if (first_held_pos.x != -1) {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderRect(renderer, &sel_rect);
	}

	SDL_SetRenderDrawColor(renderer, 0x10, 0x2a, 0x6e, 0xFF);
	SDL_RenderFillRect(renderer, RECT(0,0,NATIVE_WIDTH,20));
	SDL_RenderFillRect(renderer, RECT(0,NATIVE_HEIGHT-20,NATIVE_WIDTH,20));

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});
	char msg[512];
	snprintf(msg, 511, "Score: %d", score);
	draw_text(renderer, msg, 0,0, 2);

	button(renderer, &pause_button);
}
