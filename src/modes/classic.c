#include "classic.h"
#include "media/textures.h"

GM_CLICK(classic, GM_Classic)

static void sm_draw(SDL_Renderer *renderer, SDL_FRect rect) {
	SDL_FRect clock_rect = {
		rect.x + 140,
		rect.y + 10,
		64, 64
	};

	SDL_RenderTexture(renderer, textures_get(TEX_CLOCK), NULL,
		&clock_rect);
}

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
	classic_click,
	NULL,
	sm_draw
};
