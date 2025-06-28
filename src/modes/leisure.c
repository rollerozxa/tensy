#include "leisure.h"

GM_CLICK(leisure, GM_Leisure)

GameMode leisure_gamemode = {
	"Leisure",
	{
		"Infinite time,",
		"infinite shuffles,",
		"infinite calm."
	},
	{
		{2,2,2,2,0,0},
		{2,2,2,2,0,0},
		{2,2,1,1,1,1},
		{2,2,1,1,1,1},
	},
	false,
	leisure_click
};
