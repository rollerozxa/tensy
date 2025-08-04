#include "color.h"

/*
 * This source file had to be renamed to color.c in order to prevent Tensy
 * from falling under legislation of the 2023 Online Safety Act.
 *
 * (I kid...)
 */

/**
 * Get a nice unique colour for each number
 */
SDL_Color color_numbers(int num) {
	switch (num) {
		case 1: return (SDL_Color){0xFF, 0x98, 0x8F};
		case 2: return (SDL_Color){0xFF, 0xB1, 0x69};
		case 3: return (SDL_Color){0xFF, 0xF0, 0x69};
		case 4: return (SDL_Color){0xA0, 0xFF, 0x69};
		case 5: return (SDL_Color){0x69, 0xFF, 0x73};
		case 6: return (SDL_Color){0x69, 0xFF, 0xD2};
		case 7: return (SDL_Color){0x69, 0xBB, 0xFF};
		case 8: return (SDL_Color){0x9D, 0x7D, 0xFF};
		case 9: return (SDL_Color){0xFF, 0x7D, 0xE7};
	}

	return (SDL_Color){0xFF, 0xFF, 0xFF};
}

unsigned int color_sdl_to_bitpack(SDL_Color color) {
	return (color.r << 16)
		 + (color.g << 8)
		 + color.b;
}
