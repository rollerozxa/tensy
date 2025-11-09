#include "draw.h"
#include "gamemode.h"
#include "board.h"
#include "media/textures.h"
#include <SDL3/SDL.h>

GM_CLICK(infinite, GM_Infinite)

static void sm_draw(SDL_FRect rect) {
	SDL_FRect inf_rect = {
		rect.x + 142,
		rect.y + 13,
		64, 64
	};

	draw_texture(TEX_INFINITE, NULL, &inf_rect);
}

GameMode infinite_gamemode = {
    GM_KEY('I', 'N', 'F', 'I'),
    "Infinite",
    {
        "Numbers fall from",
        "above to replace",
        "removed ones.",
        "Forever."
    },
    {
		{2,2,3,1,0,0},
		{1,2,2,2,0,0},
		{1,4,2,4,1,3},
		{2,3,2,3,4,3},
    },
    .sm_click = infinite_click,
    .sm_draw = sm_draw,
    .gravity_mode = GRA_Always,
    .time_limit = false
};
