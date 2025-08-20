#pragma once

#include <SDL3/SDL.h>

extern SDL_Renderer *renderer;

#if SDL_MINOR_VERSION < 3
// fallback for 3.2.x
#define SDL_SCALEMODE_PIXELART SDL_SCALEMODE_NEAREST
#endif

bool draw_line(float x1, float y1, float x2, float y2);
bool draw_rect(const SDL_FRect *rect);
bool draw_fill_rect(const SDL_FRect *rect);
bool draw_texture(int texture_id, const SDL_FRect *srcrect, const SDL_FRect *dstrect) ;

void draw_set_blend(bool blend);

void draw_set_color(unsigned int color);
void draw_set_color_alpha(unsigned int color);

void draw_box(SDL_FRect *rect);
void draw_box_hover(SDL_FRect *rect);
void draw_box_active(SDL_FRect *rect);
void draw_box_disabled(SDL_FRect *rect);

void draw_border_box(SDL_FRect *rect, unsigned int bgcolor, unsigned int bordercolor, int bordersize);

void draw_translucent_overlay(void);
SDL_FRect draw_centered_fill_rect(SDL_FPoint dimensions);

void draw_tiled_bg(double bgpan);
