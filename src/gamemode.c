#include "gamemode.h"
#include "gamestate.h"
#include "modes/classic.h"
#include "modes/five.h"
#include "modes/leisure.h"
#include "modes/lucky.h"
#include "scene.h"

GameMode gamemodes[100];
static int registered_gamemodes = 0;

static void reg(GameMode mode) {
	gamemodes[registered_gamemodes] = mode;
	registered_gamemodes++;
}

void init_gamemodes(void) {
	reg(classic_gamemode);
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
	switch_scene("gameconfig");
}
