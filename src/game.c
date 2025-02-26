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
	Cell **p;
	int w;
	int h;
	float scale;
	float cell_size;
	float full_width;
	float full_height;
} Board;

static Board board = {NULL, 20, 10, 2.5};

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

static SDL_Point board_to_screen_coord(int x, int y) {
	SDL_Point point = {
		(NATIVE_WIDTH - board.full_width) / 2,
		(NATIVE_HEIGHT - board.full_height) / 2
	};
	point.x += x * board.cell_size;
	point.y += y * board.cell_size;

	return point;
}

static void board_create() {
	board.p = malloc(sizeof(Cell *) * board.w);
	board.cell_size = board.scale * 10;
	board.full_width = board.w * board.cell_size;
	board.full_height = board.h * board.cell_size;

	for (size_t x = 0; x < board.w; x++) {
		board.p[x] = malloc(sizeof(Cell) * board.h);

		for (size_t y = 0; y < board.h; y++) {
			board.p[x][y].number = SDL_rand(9) + 1;
			board.p[x][y].removed = false;
		}
	}
}

void game_init(void) {
	board_create();
}

void game_event(const SDL_Event *ev) {
	#define CELL_X (ev->motion.x - (float)(NATIVE_WIDTH - board.full_width) / 2) / board.cell_size
	#define CELL_Y (ev->motion.y - (float)(NATIVE_HEIGHT - board.full_height) / 2) / board.cell_size

	if (has_overlay()) return;

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int cx = CELL_X;
		int cy = CELL_Y;

		if (SDL_clamp(cx, 0, board.w-1) == cx && SDL_clamp(cy, 0, board.h-1) == cy) {
			first_held_pos = (struct SDL_Point){cx, cy};
			current_held_pos = first_held_pos;
			helddown = true;
		}
	} else if (ev->type == SDL_EVENT_MOUSE_MOTION) {
		if (helddown) {
			current_held_pos.x = CELL_X;
			current_held_pos.y = CELL_Y;

			current_held_pos.x = SDL_clamp(current_held_pos.x, 0, board.w-1);
			current_held_pos.y = SDL_clamp(current_held_pos.y, 0, board.h-1);

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
		abs(current_held_point.x - first_held_point.x) + board.cell_size,
		abs(current_held_point.y - first_held_point.y) + board.cell_size
	};

	if (first_held_pos.x != -1) {
		if (held_sum == 10) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0xA0, 0x00, 0xFF);

			SDL_RenderFillRect(renderer, &sel_rect);
		}
	}

	for (size_t x = 0; x < board.w; x++) {
	for (size_t y = 0; y < board.h; y++) {
		if (settings()->coloured_numbers)
			set_font_color(num_to_colour(board.p[x][y].number));

		SDL_Point point = board_to_screen_coord(x,y);
		point.x += 3;
		point.y -= 1;

		if (!board.p[x][y].removed)
			draw_char_shadow(renderer, board.p[x][y].number + '0',
				point.x, point.y, board.scale);
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
