#pragma once

#include <SDL3/SDL.h>

#define CLR_WHITE (SDL_Color){0xFF, 0xFF, 0xFF}
#define CLR_RED   (SDL_Color){0xFF, 0x00, 0x00}
#define CLR_BLACK (SDL_Color){0x00, 0x00, 0x00}

unsigned int parse_hex_color_bitpack(const char *hex);
void color_to_hex6(unsigned int color, char *out);

void color_load_custom_numbers(void);

SDL_Color color_numbers(int num);

unsigned int color_sdl_to_bitpack(SDL_Color color);
SDL_Color color_bitpack_to_sdl(unsigned int color);

SDL_Color HSVtoRGB(double h, double s, double v);
