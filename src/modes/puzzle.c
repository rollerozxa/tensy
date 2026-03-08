#include "draw.h"
#include "gamemode.h"
#include "scene.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

static void puzzle_click(void) {
	switch_gamemode(GM_Puzzle);
	// Puzzle mode uses a custom level-select scene rather than the standard config
	scene_switch("puzzle_select");
}

GameMode puzzle_gamemode = {
	GM_KEY('P','U','Z','L'),
	"Puzzle",
	{
		"Play predefined",
		"puzzle levels.",
		"Select a level to",
		"start."
	},
	{
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},
	},
	.sm_click=puzzle_click
};
