#include <stdbool.h>

#include "gamemode.h"

GM_CLICK(gravity, GM_Gravity)

GameMode gravity_gamemode = {
	"Gravity",
	{
		"Numbers will fall",
		"down, as if they ",
		"are affected by",
		"gravity."
	},
	{
		{2,0,0,0,0,0},
		{1,0,0,0,1,1},
		{1,4,0,0,1,3},
		{2,3,2,3,4,3},
	},
	false,
	gravity_click,
	.gravity_mode=GRA_Always,
	.time_limit=true
};
