#pragma once

#include <SDL3/SDL.h>

#define GLYPH_WIDTH 6
#define GLYPH_HEIGHT 12

SDL_Texture *load_font(SDL_Renderer *renderer);

SDL_Color get_font_color(void);
void set_font_color(SDL_Color clr);

void draw_char(SDL_Renderer *renderer, unsigned char character, float cx, float cy, float scale);
void draw_char_shadow(SDL_Renderer *renderer, unsigned char character, float cx, float cy, float scale);

void draw_text(SDL_Renderer *renderer, const char *text, float x, float y, float scale);
void draw_text_shadow(SDL_Renderer *renderer, const char *text, float x, float y, float scale);

void draw_text_shadow_centered(SDL_Renderer *renderer, const char *text, SDL_FRect *rect, float scale);

SDL_FRect calculate_text_rect(const char *text, float scale);
