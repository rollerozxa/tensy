#pragma once

#include "gamemode.h"
#include "gamestate.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

#define MAX_HIGHSCORES 20

typedef struct {
	unsigned int score;
	char name[12];
} Highscore;

extern Highscore highscores[gamemode_count][4][MAX_HIGHSCORES];

void highscore_register(Game state, const char *name);

void highscores_clear(void);

bool highscores_file_save(void);
bool highscores_file_load(void);
