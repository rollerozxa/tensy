#pragma once

#include "board.h"
#include "gamemode.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	Board board;
	int score;
	float time_left, total_time;
	enum GameMode mode;
	bool loaded_existing;
	int shuffles;
	bool dirty;
	bool dead;
	// Random identifier used to check if an active game corresponds to the current saved game
	uint64_t identifier;
} Game;

// Smaller state struct used for undo history
typedef struct {
	Board board;
	int score;
	int shuffles;
} Checkpoint;

extern Game game;

void gamestate_checkpoint(void);
bool gamestate_has_undo(void);
void gamestate_undo(void);
void gamestate_clear(void);
void gamestate_traverse(void);

void gamestate_gameover(void);
