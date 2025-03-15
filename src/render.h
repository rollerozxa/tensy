#pragma once

#include <SDL3/SDL.h>

void set_draw_color(SDL_Renderer *renderer, unsigned int color);
void set_draw_color_alpha(SDL_Renderer *renderer, unsigned int color);

void draw_box(SDL_Renderer *renderer, SDL_FRect *rect);
void draw_box_hover(SDL_Renderer *renderer, SDL_FRect *rect);
void draw_box_active(SDL_Renderer *renderer, SDL_FRect *rect);
