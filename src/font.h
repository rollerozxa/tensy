#pragma once

#include <SDL3/SDL.h>

#define GLYPH_WIDTH 6
#define GLYPH_HEIGHT 12

SDL_Color font_get_color(void);
void font_set_color(SDL_Color clr);

void font_draw_char(unsigned char character, float cx, float cy, float scale);
void font_draw_char_shadow(unsigned char character, float cx, float cy, float scale);
