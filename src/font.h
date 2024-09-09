#pragma once

#include <SDL3/SDL.h>

SDL_Texture* load_font(SDL_Renderer *renderer);

void set_font_color(SDL_Color clr);

void draw_char(SDL_Renderer *renderer, unsigned char character, int cx, int cy, int scale);
void draw_text(SDL_Renderer *renderer, const char* text, int x, int y, int scale);
