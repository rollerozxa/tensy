#pragma once

#include <SDL3/SDL.h>

#define CLR_WHITE (SDL_Color){0xFF, 0xFF, 0xFF}
#define CLR_RED   (SDL_Color){0xFF, 0x00, 0x00}
#define CLR_BLACK (SDL_Color){0x00, 0x00, 0x00}

SDL_Color color_numbers(int num);

unsigned int color_sdl_to_bitpack(SDL_Color color);
SDL_Color color_bitpack_to_sdl(unsigned int color);
