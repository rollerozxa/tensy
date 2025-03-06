#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

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
	SDL_FRect rect;
} Board;

extern Board board;

void board_change_size(int w, int h, float scale);
void board_reset(void);

SDL_Point board_to_screen_coord(int x, int y);
