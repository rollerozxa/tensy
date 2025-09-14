#pragma once

#include "gui/button.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	char number;
	bool removed;
} Cell;

typedef struct {
	bool animating;
	bool returning;
	float timer;
	float duration;
} BoardAnimation;

typedef struct {
	Cell **p;
	int w;
	int h;
	float scale;
	float cell_size;
	SDL_FRect rect;
	SDL_FPoint rect_offset;
	int _allocated_columns;
	bool physics;
	BoardAnimation anim;
	int boardsize;
} Board;

typedef struct {
	int w;
	int h;
	float scale;
	int seconds;
	const char *name;
	Button button; // XXX: move this elsewhere
} BoardSize;

static BoardSize board_sizes[] = {
	{15, 8,  3,   60*3,  "Small", },
	{20, 10, 2.5, 60*5,  "Medium", },
	{30, 15, 2,   60*7,  "Big", },
	{60, 30, 1,   60*10, "Hyuge!", }
};
static size_t num_board_sizes = sizeof(board_sizes) / sizeof(board_sizes[0]);

void board_change_size(Board *board, int w, int h, float scale);
void board_change_width(Board *board, int w);
void board_change_height(Board *board, int h);
void board_change_scale(Board *board, float scale);

void board_reset(Board *board);
void board_zerofill(Board *board);

void board_update(Board *board, float dt);
void board_draw(Board *board, bool colored_numbers);

SDL_Point board_to_screen_coord(Board *board, int x, int y);

void board_shuffle(Board *board);
void board_shuffle_animated(Board *board, float duration);

void board_copy(Board *dst, Board *src);
// Deallocate any heap allocated stuff in board
void board_free(Board *board);
