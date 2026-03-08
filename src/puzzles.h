#pragma once

typedef struct {
	char **board;
	float boardsize;
	int width;
	int height;
} Puzzle;

int puzzle_get_count(void);

void puzzles_load(void);

Puzzle *puzzles();
