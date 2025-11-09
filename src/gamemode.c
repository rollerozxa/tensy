#include "gamemode.h"
#include "gamestate.h"
#include "scene.h"
#include <stdint.h>

extern GameMode classic_gamemode;
extern GameMode five_gamemode;
extern GameMode gravity_gamemode;
extern GameMode infinite_gamemode;
extern GameMode leisure_gamemode;
extern GameMode lucky_gamemode;
extern GameMode noise_gamemode;

GameMode gamemodes[100];
static int registered_gamemodes = 0;

static void reg(GameMode mode) {
	gamemodes[registered_gamemodes] = mode;
	registered_gamemodes++;
}

void init_gamemodes(void) {
	reg(classic_gamemode);
	reg(gravity_gamemode);
	reg(infinite_gamemode);
	//reg(noise_gamemode);
	reg(leisure_gamemode);
	reg(lucky_gamemode);
	reg(five_gamemode);
}

void switch_gamemode(enum GameMode mode) {
	game.mode = mode;
}

GameMode current_gamemode(void) {
	return gamemodes[game.mode];
}

void gamemode_continue(void) {
	// for if we need to change where to go after a gamemode is selected
	scene_switch("gameconfig");
}

int gamemode_get_index_by_key(int key) {
	for (int i = 0; i < gamemode_count; i++)
		if (gamemodes[i].key == key)
			return i;

	return -1; // Not found
}
