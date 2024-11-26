#pragma once

#include <SDL3/SDL.h>

void SDLex_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color);

void draw_box(SDL_Renderer *renderer, SDL_FRect *rect);
void draw_box_hover(SDL_Renderer *renderer, SDL_FRect *rect);
void draw_box_active(SDL_Renderer *renderer, SDL_FRect *rect);
