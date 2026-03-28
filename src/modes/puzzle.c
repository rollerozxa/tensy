#include "color.h"
#include "draw.h"
#include "font.h"
#include "gamemode.h"
#include "media/textures.h"
#include "scene.h"
#include "text.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

static void puzzle_click(void) {
	switch_gamemode(GM_Puzzle);
	// Puzzle mode uses a custom level-select scene rather than the standard config
	scene_switch("puzzle_select");
}

static void sm_draw(SDL_FRect rect) {
	SDL_FRect inf_rect = {
		rect.x + (rect.w - 64) / 2,
		rect.y + 32,
		64, 64
	};

	draw_texture(TEX_PUZZLE, NULL, &inf_rect);

	font_set_color(color_bitpack_to_sdl(0xFFD700));
	inf_rect.y += 80;
	text_draw_shadow_centered("(BETA)", &inf_rect, 2.0f);
	font_set_color(CLR_WHITE);
}

GameMode puzzle_gamemode = {
	GM_KEY('P','U','Z','L'),
	"Puzzle",
	{
		"Play predefined",
		"puzzle levels.",
		"Select a level to",
		"start."
	},
	{
		{9,6,0,0,4,2},
		{1,0,0,0,0,8},
		{8,0,0,0,0,1},
		{2,4,0,0,6,9},
	},
	.sm_click=puzzle_click,
	.sm_draw=sm_draw,
};
