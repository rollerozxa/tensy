#include "raccoon.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include <math.h>

#define RACCOON_WIDTH 16
#define RACCOON_HEIGHT 16

const static unsigned char palette[5][3] = {
	{0x00, 0x00, 0x00},
	{0x41, 0x33, 0x26},
	{0x7d, 0x63, 0x4f},
	{0x8b, 0x91, 0x6f},
	{0xe0, 0xd2, 0xc0}
};

const static unsigned char image[RACCOON_WIDTH][RACCOON_HEIGHT] = {
	{0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
	{0,1,4,1,0,1,1,1,1,1,0,0,1,4,1,0},
	{1,4,4,1,0,1,2,2,2,1,1,1,1,4,4,1},
	{1,4,3,1,0,1,2,2,2,2,2,2,1,3,4,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{0,1,4,4,4,4,4,4,4,4,4,4,4,4,1,0},
	{1,4,4,4,3,3,3,3,3,3,3,3,4,4,4,1},
	{1,4,3,3,3,1,3,3,3,3,1,3,3,3,4,1},
	{1,4,3,3,1,3,1,3,3,1,3,1,3,3,4,1},
	{1,4,3,3,3,3,3,3,3,3,3,3,3,3,4,1},
	{1,4,4,4,3,3,3,3,3,3,3,3,4,4,4,1},
	{1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,1},
	{1,4,4,3,4,4,1,4,4,4,4,4,3,4,4,1},
	{0,1,4,4,4,4,4,1,1,1,4,4,4,4,1,0},
	{0,0,1,4,4,4,4,4,4,4,4,4,4,1,0,0},
	{0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
};

static int step = 0;
const static int bs = 10;

void raccoon_draw(SDL_Renderer *renderer, SDL_FPoint *point) {
	SDL_Color idx[5];

	// Prepare SDL_Color array and textures for each color
	for (int i = 0; i < 5; ++i)
		idx[i] = (SDL_Color){palette[i][0], palette[i][1], palette[i][2], 255};

	for (int y = 0; y < RACCOON_HEIGHT; y++) {
		for (int x = 0; x < RACCOON_WIDTH; x++) {
			int currentColor = image[y][x];
			if (currentColor == 0)
				continue;

			draw_set_color(renderer, color_sdl_to_bitpack(idx[currentColor]));
			SDL_RenderFillRect(renderer, &RECT(
				point->x + sin((double)(step+y*3)/15)*7+bs*x,
				point->y + bs*y, bs, bs));
		}
	}

	step++;
}
