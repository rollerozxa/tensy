#include "lucky.h"

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "board.h"

GM_CLICK(lucky, GM_Lucky)

static int calc_cell_number(Board *board, int x, int y) {
	if (y == 0 || x == 0)
		return SDL_rand(9) + 1;
	else {
		if (SDL_rand(2) == 0)
			return 10 - board->p[x][y-1].number;
		else
			return 10 - board->p[x-1][y].number;
	}
}

GameMode lucky_gamemode = {
	"Lucky",
	{
		"I'm feeling lucky!",
		"",
		""
	},
	{
		{6,7,3,9,7,1},
		{4,3,7,1,3,9},
		{6,7,3,9,7,1},
		{4,3,7,1,3,9},
	},
	false,
	lucky_click,
	.calc_cell_number=calc_cell_number
};
