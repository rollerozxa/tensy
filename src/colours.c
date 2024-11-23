#include "colours.h"

SDL_Color num_to_colour(int num) {
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