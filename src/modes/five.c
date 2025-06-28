#include "five.h"
#include "gamesettings.h"

GM_CLICK(five, GM_Five)

static bool five_enabled(void) {
	return settings()->secret_five;
}

GameMode five_gamemode = {
	"Five",
	{
		"What?? This isn't",
		"even a real game",
		"mode!?"
	},
	{
		{5,5,5,5,5,5},
		{5,5,5,5,5,5},
		{5,5,5,5,5,5},
		{5,5,5,5,5,5},
	},
	false,
	five_click,
	five_enabled
};
