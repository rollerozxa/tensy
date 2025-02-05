#include "game.h"
#include "consts.h"
#include "colours.h"
#include "font.h"
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

static Cell board[BOARD_W][BOARD_H];

static bool helddown = false;
static SDL_Point first_held_pos = {-1,-1};
static SDL_Point current_held_pos = {-1,-1};
static int held_sum = -1;

static int score = 0;

static int calculate_sum(void) {
	int sum = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
		if (!board[x][y].removed)
			sum += board[x][y].number;
	}
	}

	return sum;
}

static void board_physics(void) {
	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
	for (int y = BOARD_H - 1; y >= 0; y--) {
		if (!board[x][y].removed)
			continue;

		int src_y = y - 1;
		while (src_y >= 0 && board[x][src_y].removed) {
			src_y--;
		}

		if (src_y >= 0) {
			board[x][y].number = board[x][src_y].number;
			board[x][y].removed = false;
			board[x][src_y].removed = true;
		} else {
			board[x][y].number = 0;
			board[x][y].removed = true;
		}
	}
	}
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
	}
	}

	if (settings()->board_physics)
		board_physics();

	score += sum * (removed_cells-1);
}

void game_init(void) {
	for (size_t x = 0; x < BOARD_W; x++) {
	for (size_t y = 0; y < BOARD_H; y++) {
		board[x][y].number = SDL_rand(9) + 1;
		board[x][y].removed = false;
	}
	}
}

void game_event(const SDL_Event *ev) {
	#define CELL_X floorf(ev->motion.x / CELL_SIZE) - 1
	#define CELL_Y floorf((ev->motion.y - 10) / CELL_SIZE) - 1

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
		if (ev->key.key == SDLK_AC_BACK) {
			switch_scene("mainmenu");
		}
	}
}

void game_update(void) {


}

void game_draw(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0x1f, 0x3f, 0x8f, 0xFF);

	SDL_RenderClear(renderer);

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});

	SDL_FRect sel_rect = {
		fminf((first_held_pos.x + 1) * CELL_SIZE, (current_held_pos.x + 1) * CELL_SIZE),
		fminf((first_held_pos.y + 1.5) * CELL_SIZE, (current_held_pos.y + 1.5) * CELL_SIZE),
		abs((current_held_pos.x - first_held_pos.x) * CELL_SIZE) + CELL_SIZE,
		abs((current_held_pos.y - first_held_pos.y) * CELL_SIZE) + CELL_SIZE
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

		if (!board[x][y].removed)
			draw_char_shadow(renderer, board[x][y].number + '0',
				3 + (x + 1) * CELL_SIZE, (y + 1.5) * CELL_SIZE - 1, BOARD_SCALE);
	}
	}

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
}
