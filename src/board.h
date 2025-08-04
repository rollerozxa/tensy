#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

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
} Board;

void board_change_size(Board *board, int w, int h, float scale);
void board_change_width(Board *board, int w);
void board_change_height(Board *board, int h);
void board_change_scale(Board *board, float scale);

void board_reset(Board *board);
void board_zerofill(Board *board);

void board_update(Board *board, float dt);
void board_draw(Board *board, SDL_Renderer *renderer, bool colored_numbers);

SDL_Point board_to_screen_coord(Board *board, int x, int y);

void board_shuffle(Board *board);
void board_shuffle_animated(Board *board, float duration);

void board_copy(Board *dst, Board *src);
// Deallocate any heap allocated stuff in board
void board_free(Board *board);
