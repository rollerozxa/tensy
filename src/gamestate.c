#include "gamestate.h"
#include "gamemode.h"

Game game = {
	.board = {NULL, 30, 15, 2},
	.score = 0,
	.time_left = 0.f,
	.total_time = 0.f,
	.mode = GM_Leisure,
	.loaded_existing = false,
	.shuffles = 0
};
