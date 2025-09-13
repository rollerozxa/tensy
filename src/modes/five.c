#include "five.h"

#include <stdbool.h>

#include "board.h"
#include "gamesettings.h"

GM_CLICK(five, GM_Five)

static bool five_enabled(void) {
	return settings_getflag(FLAG_SECRET_FIVE);
}

static int calc_cell_number(Board *board, int x, int y) {
	return 5;
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
	five_enabled,
	.calc_cell_number=calc_cell_number
};
