#include "draw.h"
#include "gamemode.h"
#include "media/textures.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stddef.h>

GM_CLICK(classic, GM_Classic)

static void sm_draw(SDL_FRect rect) {
	SDL_FRect clock_rect = {
		rect.x + 140,
		rect.y + 10,
		64, 64
	};

	draw_texture(TEX_CLOCK, NULL, &clock_rect);
}

GameMode classic_gamemode = {
	GM_KEY('C', 'L', 'S', 'C'),
	"Classic",
	{
		"Collect as much ",
		"score as possible",
		"within the time",
		"limit."
	},
	{
		{7,3,1,2,0,0},
		{2,8,7,6,0,0},
		{1,4,5,4,7,3},
		{2,9,1,3,4,3},
	},
	false,
	classic_click,
	NULL,
	sm_draw,
	.time_limit=true
};
