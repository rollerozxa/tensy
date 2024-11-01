#include "game.h"
#include "consts.h"
#include "colours.h"
#include "font.h"

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
#define BOARD_H 16

Cell board[BOARD_W][BOARD_H];

bool helddown = false;
SDL_Point first_held_pos = {-1,-1};
SDL_Point current_held_pos = {-1,-1};
int held_sum = -1;

int score = 0;

int calculate_sum() {
	int sum = 0;

	for (size_t x = fminf(first_held_pos.x, current_held_pos.x); x <= fmaxf(current_held_pos.x, first_held_pos.x); x++) {
		for (size_t y = fminf(first_held_pos.y, current_held_pos.y); y <= fmaxf(current_held_pos.y, first_held_pos.y); y++) {
			if (!board[x][y].removed)
				sum += board[x][y].number;
		}
	}

	return sum;
}

void do_move() {
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

#ifdef BOARD_PHYSICS
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
#endif

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
	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int cx = floorf(ev->button.x / 20)-1;
		int cy = floorf(ev->button.y / 20)-1;

		if (SDL_clamp(cx, 0, 30-1) == cx && SDL_clamp(cy, 0, 16-1) == cy) {
			first_held_pos = (struct SDL_Point){cx,cy};
			current_held_pos = first_held_pos;
			helddown = true;
		}
	} else if (ev->type == SDL_EVENT_MOUSE_MOTION) {
		if (helddown) {
			current_held_pos.x = floorf(ev->motion.x / 20)-1;
			current_held_pos.y = floorf(ev->motion.y / 20)-1;

			current_held_pos.x = SDL_clamp(current_held_pos.x, 0, 30-1);
			current_held_pos.y = SDL_clamp(current_held_pos.y, 0, 16-1);

			held_sum = calculate_sum();
		}
	} else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP) {
		do_move();

		first_held_pos = (struct SDL_Point){-1,-1};
		current_held_pos = (struct SDL_Point){-1,-1};

		held_sum = -1;
		helddown = false;
	}
}

void game_update(void) {


}

void game_draw(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0x1f, 0x3f, 0x8f, 0xFF);

	SDL_RenderClear(renderer);

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});

	if (first_held_pos.x != -1) {
		if (held_sum == 10) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0xA0, 0x00, 0xFF);
			SDL_RenderFillRect(renderer,
				RECT(fminf((first_held_pos.x+1)*20, (current_held_pos.x+1)*20),
				fminf((first_held_pos.y+1)*20, (current_held_pos.y+1)*20),
				abs((current_held_pos.x - first_held_pos.x) * 20)+20,
				abs((current_held_pos.y - first_held_pos.y) * 20)+20));
		}
	}

	for (size_t x = 0; x < BOARD_W; x++) {
		for (size_t y = 0; y < BOARD_H; y++) {
#ifdef COLOURED_NUMBERS
			set_font_color(num_to_colour(board[x][y].number));
#endif
			if (!board[x][y].removed)
				draw_char_shadow(renderer, board[x][y].number+'0', 3+(x+1)*20, (y+1)*20-1, 2);
		}
	}

	if (first_held_pos.x != -1) {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderRect(renderer,
			RECT(fminf((first_held_pos.x+1)*20, (current_held_pos.x+1)*20),
			fminf((first_held_pos.y+1)*20, (current_held_pos.y+1)*20),
			abs((current_held_pos.x - first_held_pos.x) * 20)+20,
			abs((current_held_pos.y - first_held_pos.y) * 20)+20));
	}

	SDL_SetRenderDrawColor(renderer, 0x10, 0x2a, 0x6e, 0xFF);
	SDL_RenderFillRect(renderer, RECT(0,0,NATIVE_WIDTH,20));
	SDL_RenderFillRect(renderer, RECT(0,NATIVE_HEIGHT-20,NATIVE_WIDTH,20));

	set_font_color((SDL_Color){0xFF, 0xFF, 0xFF});
	char msg[512];
	snprintf(msg, 511, "Score: %d", score);
	draw_text(renderer, msg, 0,0, 2);
}
