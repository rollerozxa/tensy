#include "render.h"
#include "consts.h"

void set_draw_color(SDL_Renderer *renderer, unsigned int color) {
	SDL_SetRenderDrawColor(renderer,
		(color >> 16) & 0xFF,
		(color >> 8)  & 0xFF,
		color & 0xFF,
		SDL_ALPHA_OPAQUE);
}

void set_draw_color_alpha(SDL_Renderer *renderer, unsigned int color) {
	SDL_SetRenderDrawColor(renderer,
		(color >> 24) & 0xFF,
		(color >> 16) & 0xFF,
		(color >> 8)  & 0xFF,
		color & 0xFF);
}

void draw_box(SDL_Renderer *renderer, SDL_FRect *rect) {
	set_draw_color(renderer, 0x131313);
	SDL_RenderFillRect(renderer, &RECT(rect->x, rect->y, rect->w, rect->h));

	set_draw_color(renderer, 0xFFFFFF);
	SDL_RenderFillRect(renderer, &RECT(rect->x+1, rect->y+1, rect->w-2, rect->h-2));

	set_draw_color(renderer, 0xC6C6C6);
	SDL_RenderFillRect(renderer, &RECT(rect->x+1, rect->y+rect->h-2, 1, 1));
	SDL_RenderFillRect(renderer, &RECT(rect->x+rect->w-2, rect->y+1, 1, 1));

	set_draw_color(renderer, 0x585658);
	SDL_RenderFillRect(renderer, &RECT(rect->x+2, rect->y+2, rect->w-3, rect->h-3));

	set_draw_color(renderer, 0xA2A2A2);
	SDL_RenderFillRect(renderer, &RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}

void draw_box_hover(SDL_Renderer *renderer, SDL_FRect *rect) {
	draw_box(renderer, rect);

	set_draw_color(renderer, 0xA6ADDB);
	SDL_RenderFillRect(renderer, &RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}

void draw_box_active(SDL_Renderer *renderer, SDL_FRect *rect) {
	draw_box(renderer, rect);

	set_draw_color(renderer, 0x747474);
	SDL_RenderFillRect(renderer, &RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}
