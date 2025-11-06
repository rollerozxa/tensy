#include "board.h"
#include "gamemode.h"
#include "gamesettings.h"
#include <stdbool.h>

GM_CLICK(five, GM_Five)

static bool five_enabled(void) {
	return settings_getflag(FLAG_SECRET_FIVE);
}

static int calc_cell_number(Board *board, int x, int y) {
	return 5;
}

GameMode five_gamemode = {
	GM_KEY('F', 'I', 'V', 'E'),
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
