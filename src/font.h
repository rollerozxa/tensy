#pragma once

#include <SDL3/SDL.h>

#define GLYPH_WIDTH 6
#define GLYPH_HEIGHT 12

SDL_Texture* load_font(SDL_Renderer *renderer);

SDL_Color get_font_color();
void set_font_color(SDL_Color clr);

void draw_char(SDL_Renderer *renderer, unsigned char character, int cx, int cy, int scale);
void draw_char_shadow(SDL_Renderer *renderer, unsigned char character, int cx, int cy, int scale);

void draw_text(SDL_Renderer *renderer, const char* text, int x, int y, int scale);
void draw_text_shadow(SDL_Renderer *renderer, const char* text, int x, int y, int scale);
