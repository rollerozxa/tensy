#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "board.h"

#define GM_CLICK(name, enum) \
	static void name##_click(void) { \
		switch_gamemode(enum); \
		gamemode_continue(); \
	}

enum GameMode {
	GM_Classic,
	GM_Gravity,
	GM_Leisure,
	GM_Lucky,
	GM_Five,
	gamemode_count
};

typedef enum GravityMode {
	GRA_Never,
	GRA_Always,
	GRA_Ask
} GravityMode;

typedef struct {
	const char *name;
	const char *description[4];
	char board[4][7];
	bool disabled;
	void (*sm_click)(void);
	bool (*sm_enabled)(void);
	void (*sm_draw)(SDL_FRect rect);
	int (*calc_cell_number)(Board *board, int x, int y);
	GravityMode gravity_mode;
	bool time_limit;
} GameMode;

extern GameMode gamemodes[];

void init_gamemodes(void);

void switch_gamemode(enum GameMode mode);

void gamemode_continue(void);

GameMode current_gamemode(void);
