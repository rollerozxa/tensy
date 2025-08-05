#pragma once

#include <SDL3/SDL.h>

extern SDL_Renderer *renderer;



bool draw_texture(int texture_id, const SDL_FRect *srcrect, const SDL_FRect *dstrect) ;

void draw_set_color(unsigned int color);
void draw_set_color_alpha(unsigned int color);

void draw_box(SDL_FRect *rect);
void draw_box_hover(SDL_FRect *rect);
void draw_box_active(SDL_FRect *rect);

void draw_translucent_overlay(void);
SDL_FRect draw_centered_fill_rect(SDL_FPoint dimensions);
