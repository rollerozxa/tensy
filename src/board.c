#include "board.h"
#include "color.h"
#include "consts.h"
#include "font.h"
#include "gamemode.h"
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

static int calc_cell_number(Board *board, int x, int y) {
	if (current_gamemode().calc_cell_number)
		return current_gamemode().calc_cell_number(board, x, y);
	else
		return SDL_rand(9) + 1;
}

void board_reset(Board *board) {
	if (board->p != NULL)
		board_cleanup(board);

	board->p = malloc(sizeof(Cell *) * board->w);

	for (int x = 0; x < board->w; x++) {
		board->p[x] = malloc(sizeof(Cell) * board->h);

		for (int y = 0; y < board->h; y++) {
			board->p[x][y].number = calc_cell_number(board, x, y);
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

void board_update(Board *board, float dt) {
	if (!board->anim.animating)
		return;

	board->anim.timer += dt;

	if (board->anim.timer >= board->anim.duration) {
		board->anim.timer = 0.0f;

		if (!board->anim.returning) {
			board_shuffle(board);
			board->anim.returning = true;
		} else {
			board->anim.animating = false;
		}
	}
}

void board_draw(Board *board, bool colored_numbers) {
	SDL_FPoint center = {
		board->rect.x + board->rect.w / 2.0f,
		board->rect.y + board->rect.h / 2.0f
	};

	float t = board->anim.timer / board->anim.duration;
	if (t > 1.0f) t = 1.0f;
	if (board->anim.returning) t = 1.0f - t; // reverse direction

	for (int x = 0; x < board->w; x++) {
	for (int y = 0; y < board->h; y++) {
		if (board->p[x][y].removed)
			continue;

		if (colored_numbers)
			font_set_color(color_numbers(board->p[x][y].number));

		SDL_Point point = board_to_screen_coord(board, x, y);
		SDL_FPoint pos = {
			point.x + 3,
			point.y - 1
		};

		if (board->anim.animating) {
			pos.x = pos.x * (1.0f - t) + center.x * t;
			pos.y = pos.y * (1.0f - t) + center.y * t;
		}

		font_draw_char_shadow(board->p[x][y].number + '0',
			pos.x, pos.y, board->scale);
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

void board_shuffle_animated(Board *board, float duration) {
	if (board->anim.animating)
		return;

	board->anim = (BoardAnimation){true, false, 0.0f, duration};
}

void board_copy(Board *dst, Board *src) {
	memcpy(dst, src, sizeof(Board));

	dst->p = malloc(sizeof(Cell *) * dst->w);

	for (int x = 0; x < dst->w; x++) {
		dst->p[x] = malloc(sizeof(Cell) * dst->h);

		for (int y = 0; y < dst->h; y++)
			memcpy(&dst->p[x][y], &src->p[x][y], sizeof(Cell));
	}
}

void board_free(Board *board) {
	if (!board->p) return;

	board_cleanup(board);
}
