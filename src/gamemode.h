#pragma once

#include <SDL3/SDL.h>

#define GM_CLICK(name, enum) \
	static void name##_click(void) { \
		switch_gamemode(enum); \
		gamemode_continue(); \
	}

enum GameMode {
	GM_Classic,
	GM_Leisure,
	GM_Five,
	gamemode_count
};

typedef struct {
	const char *name;
	const char *description[4];
	char board[4][7];
	bool disabled;
	void (*click)(void);
	bool (*enabled)(void);
} GameMode;

extern GameMode gamemodes[];

void init_gamemodes(void);

void switch_gamemode(enum GameMode mode);

void gamemode_continue(void);
