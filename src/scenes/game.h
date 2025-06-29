#pragma once

#include "board.h"
#include "scene.h"
#include "gamemode.h"
#include <SDL3/SDL.h>

extern Scene game_scene;

typedef struct {
	Board board;
	int score;
	float time_left, total_time;
	enum GameMode mode;
	bool loaded_existing;
	int shuffles;
} Game;

extern Game game;
