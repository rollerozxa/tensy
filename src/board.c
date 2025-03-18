#include "board.h"
#include "colours.h"
#include "consts.h"
#include "font.h"
#include "scene.h"
#include <stdlib.h>
#include <string.h>

Board board = {NULL, 30, 15, 2};

int allocated_columns = -1;

void board_recalculate_rect(void) {
	board.rect = (SDL_FRect){0,0,0,0};
	board.rect.w = board.w * board.cell_size;
	board.rect.h = board.h * board.cell_size;
	board.rect.x = (NATIVE_WIDTH - board.rect.w) / 2;
	board.rect.y = (NATIVE_HEIGHT - board.rect.h) / 2;
	// haha oh my god
	if (strcmp(get_current_scene(), "gameconfig") == 0) {
		board.rect.y += 50;
	}
}

void board_change_size(int w, int h, float scale) {
	board.w = w;
	board.h = h;
	board.scale = scale;
	board.cell_size = board.scale * 10;

	board_recalculate_rect();

	board_reset();
}

void board_change_width(int w) {
	board_change_size(w, board.h, board.scale);
}

void board_change_height(int h) {
	board_change_size(board.w, h, board.scale);
}

void board_change_scale(float scale) {
	board.scale = scale;
	board_recalculate_rect();
}

void board_cleanup(void) {
	SDL_assert(board.p);

	for (int x = 0; x < allocated_columns; x++)
		free(board.p[x]);

	free(board.p);
}

void board_reset(void) {
	if (board.p != NULL)
		board_cleanup();

	board.p = malloc(sizeof(Cell *) * board.w);

	for (int x = 0; x < board.w; x++) {
		board.p[x] = malloc(sizeof(Cell) * board.h);

		for (int y = 0; y < board.h; y++) {
			board.p[x][y].number = SDL_rand(9) + 1;
			board.p[x][y].removed = false;
		}
	}
	allocated_columns = board.w;
}

void board_zerofill(void) {
	for (int x = 0; x < board.w; x++) {
		for (int y = 0; y < board.h; y++) {
			board.p[x][y].number = 0;
			board.p[x][y].removed = false;
		}
	}
}

void board_draw(SDL_Renderer *renderer, bool coloured_numbers) {
	for (int x = 0; x < board.w; x++) {
	for (int y = 0; y < board.h; y++) {
		if (coloured_numbers)
			set_font_color(num_to_colour(board.p[x][y].number));

		SDL_Point point = board_to_screen_coord(x,y);
		point.x += 3;
		point.y -= 1;

		if (!board.p[x][y].removed)
			draw_char_shadow(renderer, board.p[x][y].number + '0',
				point.x, point.y, board.scale);
	}}
}

SDL_Point board_to_screen_coord(int x, int y) {
	SDL_Point point = {
		board.rect.x,
		board.rect.y
	};
	point.x += x * board.cell_size;
	point.y += y * board.cell_size;

	return point;
}
