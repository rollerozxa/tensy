#include "board.h"
#include "gamemode.h"
#include <perlin/perlin.h>
#include <SDL3/SDL.h>
#include <stdbool.h>

// Experimental gamemode using Perlin noise to generate cell numbers.
// I don't know if this is any good, but it was an interesting experiment.

#define PERLIN_SCALE 0.7f

//GM_CLICK(noise, GM_Noise)

static int calc_cell_number(Board *board, int x, int y) {
	return perlin_val_to_1_10(perlin_2d((float)x * PERLIN_SCALE, (float)y * PERLIN_SCALE));
}

static void noise_on_start(void) {
	perlin_init(SDL_rand_bits());
}

GameMode noise_gamemode = {
	"Noise",
	{
		"Come on, feel the noise.",
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
	//noise_click,
	.calc_cell_number=calc_cell_number,
	.on_start=noise_on_start,
};
