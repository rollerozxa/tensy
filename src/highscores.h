#pragma once

#include "gamemode.h"
#include "gamestate.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

#define MAX_HIGHSCORES 1024

typedef struct {
	Uint64 score;
	enum GameMode gamemode;
	char size;
	char flags;
} Highscore;

Highscore *highscores(void);

void highscore_register(Game state);

bool highscores_file_save(void);
bool highscores_file_load(void);
