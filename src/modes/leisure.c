#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stddef.h>

#include "font.h"
#include "gamemode.h"

GM_CLICK(leisure, GM_Leisure)

static void sm_draw(SDL_FRect rect) {
	SDL_FPoint z_rect = {
		rect.x + 154, rect.y + 10
	};
	font_draw_char_shadow('Z', z_rect.x, z_rect.y, 2.5);

	z_rect.x += 19;
	z_rect.y += 15;

	font_draw_char_shadow('z', z_rect.x, z_rect.y, 2.5);

	z_rect.x -= 17;
	z_rect.y += 16;

	font_draw_char_shadow('z', z_rect.x, z_rect.y, 2.5);
}

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
	leisure_click,
	NULL,
	sm_draw,
	.gravity_mode=GRA_Ask
};
