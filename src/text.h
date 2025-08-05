#pragma once

#include <SDL3/SDL.h>

void text_draw(const char *text, float x, float y, float scale);
void text_draw_shadow(const char *text, float x, float y, float scale);

void text_draw_centered(const char *text, SDL_FRect *rect, float scale);
void text_draw_shadow_centered(const char *text, SDL_FRect *rect, float scale);

SDL_FRect text_calc_rect(const char *text, float scale);
