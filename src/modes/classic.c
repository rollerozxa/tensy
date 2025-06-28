#include "classic.h"

GM_CLICK(classic, GM_Classic)

GameMode classic_gamemode = {
	"Classic",
	{
		"Collect as much ",
		"score as possible",
		"within the time",
		"limit."
	},
	{
		{2,2,3,1,0,0},
		{1,2,2,2,0,0},
		{1,4,2,4,1,3},
		{2,3,2,3,4,3},
	},
	false,
	classic_click
};
