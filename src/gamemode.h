#pragma once

#include "board.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

#define GM_CLICK(name, enum) \
	static void name##_click(void) { \
		switch_gamemode(enum); \
		gamemode_continue(); \
	}

#define GM_KEY(c1, c2, c3, c4) \
	((uint32_t)(c1) | ((uint32_t)(c2) << 8) | ((uint32_t)(c3) << 16) | ((uint32_t)(c4) << 24))

enum GameMode {
	GM_Classic,
	GM_Gravity,
	GM_Leisure,
	GM_Lucky,
	GM_Five,
	gamemode_count
};

static const char *gamemode_names[] = {
	"Classic",
	"Gravity",
	"Leisure",
	"Lucky",
	"Five"
};

typedef enum GravityMode {
	GRA_Never,
	GRA_Always,
	GRA_Ask
} GravityMode;

typedef struct {
	int key;
	const char *name;
	const char *description[4];
	char board[4][7];
	bool disabled;
	void (*sm_click)(void);
	bool (*sm_enabled)(void);
	void (*sm_draw)(SDL_FRect rect);
	int (*calc_cell_number)(Board *board, int x, int y);
	void (*on_start)(void);
	GravityMode gravity_mode;
	bool time_limit;
} GameMode;

extern GameMode gamemodes[];

void init_gamemodes(void);

void switch_gamemode(enum GameMode mode);

void gamemode_continue(void);

GameMode current_gamemode(void);

int gamemode_get_index_by_key(int key);
