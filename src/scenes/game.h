#pragma once

#include "board.h"
#include "scene.h"
#include <SDL3/SDL.h>

extern Scene game_scene;

enum GameMode {
	GM_Classic,
	GM_Leisure
};

extern int score;
extern Board board;
extern float time_left, total_time;
extern enum GameMode gamemode;
