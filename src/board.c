#include "board.h"
#include "colours.h"
#include "consts.h"
#include "font.h"
#include "scene.h"
#include <stdlib.h>
#include <string.h>

void board_recalculate_rect(Board *board) {
	board->rect = (SDL_FRect){0,0,0,0};
	board->rect.w = board->w * board->cell_size;
	board->rect.h = board->h * board->cell_size;
	board->rect.x = (NATIVE_WIDTH - board->rect.w) / 2 + board->rect_offset.x;
	board->rect.y = (NATIVE_HEIGHT - board->rect.h) / 2 + board->rect_offset.y;
}

void board_change_size(Board *board, int w, int h, float scale) {
	board->w = w;
	board->h = h;
	board->scale = scale;
	board->cell_size = board->scale * 10;

	board_recalculate_rect(board);

	board_reset(board);
}

void board_change_width(Board *board, int w) {
	board_change_size(board, w, board->h, board->scale);
}

void board_change_height(Board *board, int h) {
	board_change_size(board, board->w, h, board->scale);
}

void board_change_scale(Board *board, float scale) {
	board->scale = scale;
	board_recalculate_rect(board);
}

void board_cleanup(Board *board) {
	SDL_assert(board->p);

	for (int x = 0; x < board->_allocated_columns; x++)
		free(board->p[x]);

	free(board->p);
}

void board_reset(Board *board) {
	if (board->p != NULL)
		board_cleanup(board);

	board->p = malloc(sizeof(Cell *) * board->w);

	for (int x = 0; x < board->w; x++) {
		board->p[x] = malloc(sizeof(Cell) * board->h);

		for (int y = 0; y < board->h; y++) {
			board->p[x][y].number = SDL_rand(9) + 1;
			board->p[x][y].removed = false;
		}
	}
	board->_allocated_columns = board->w;
}

void board_zerofill(Board *board) {
	for (int x = 0; x < board->w; x++) {
		for (int y = 0; y < board->h; y++) {
			board->p[x][y].number = 0;
			board->p[x][y].removed = false;
		}
	}
}

void board_draw(Board *board, SDL_Renderer *renderer, bool coloured_numbers) {
	for (int x = 0; x < board->w; x++) {
	for (int y = 0; y < board->h; y++) {
		if (coloured_numbers)
			set_font_color(num_to_colour(board->p[x][y].number));

		SDL_Point point = board_to_screen_coord(&*&*board, x,y);
		point.x += 3;
		point.y -= 1;

		if (!board->p[x][y].removed)
			draw_char_shadow(renderer, board->p[x][y].number + '0',
				point.x, point.y, board->scale);
	}}
}

SDL_Point board_to_screen_coord(Board *board, int x, int y) {
	SDL_Point point = {
		board->rect.x,
		board->rect.y
	};
	point.x += x * board->cell_size;
	point.y += y * board->cell_size;

	return point;
}

void board_shuffle(Board *board) {
    int w = board->w;
    int h = board->h;

	// Fisher-Yates shuffle
    for (int i = w * h - 1; i > 0; --i) {
        // Current coordinates (x1, y1)
        int x1 = i / h;
        int y1 = i % h;

        // Random target index
        int j = rand() % (i + 1);
        int x2 = j / h;
        int y2 = j % h;

        // Swap elements
        Cell tmp = board->p[x1][y1];
        board->p[x1][y1] = board->p[x2][y2];
        board->p[x2][y2] = tmp;
    }
}
