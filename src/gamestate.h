#pragma once

#include "board.h"
#include "gamemode.h"

typedef struct {
	Board board;
	int score;
	float time_left, total_time;
	enum GameMode mode;
	bool loaded_existing;
	int shuffles;
} Game;

extern Game game;
