#pragma once

#include "board.h"
#include "scene.h"
#include "gamemode.h"
#include <SDL3/SDL.h>

extern Scene game_scene;

extern int score;
extern Board board;
extern float time_left, total_time;
extern enum GameMode gamemode;
extern bool loaded_existing;
extern int shuffles;
