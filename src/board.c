#include "board.h"
#include "consts.h"
#include <stdlib.h>
#include <string.h>

Board board = {NULL, 30, 15, 2};

int allocated_columns = -1;

void board_change_size(int w, int h, float scale) {
	board.w = w;
	board.h = h;
	board.scale = scale;
	board.cell_size = board.scale * 10;

	board.rect = (SDL_FRect){0,0,0,0};
	board.rect.w = board.w * board.cell_size;
	board.rect.h = board.h * board.cell_size;
	board.rect.x = (NATIVE_WIDTH - board.rect.w) / 2;
	board.rect.y = (NATIVE_HEIGHT - board.rect.h) / 2;
	
	board_reset();
}

void board_cleanup(void) {
	SDL_assert(board.p);

	for (size_t x = 0; x < allocated_columns; x++)
		free(board.p[x]);

	free(board.p);
}

void board_reset(void) {
	if (board.p != NULL)
		board_cleanup();

	board.p = malloc(sizeof(Cell *) * board.w);

	for (size_t x = 0; x < board.w; x++) {
		board.p[x] = malloc(sizeof(Cell) * board.h);

		for (size_t y = 0; y < board.h; y++) {
			board.p[x][y].number = SDL_rand(9) + 1;
			board.p[x][y].removed = false;
		}
	}
	allocated_columns = board.w;
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
