#include "render.h"
#include "consts.h"

void SDLex_SetRenderDrawColor(SDL_Renderer *renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void draw_box(SDL_Renderer *renderer, SDL_FRect *rect) {
	SDL_SetRenderDrawColor(renderer, 0x13, 0x13, 0x13, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x, rect->y, rect->w, rect->h));

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+1, rect->y+1, rect->w-2, rect->h-2));

	SDL_SetRenderDrawColor(renderer, 0xc6, 0xc6, 0xc6, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+1, rect->y+rect->h-2, 1, 1));
	SDL_RenderFillRect(renderer, RECT(rect->x+rect->w-2, rect->y+1, 1, 1));

	SDL_SetRenderDrawColor(renderer, 0x58, 0x56, 0x58, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+2, rect->y+2, rect->w-3, rect->h-3));

	SDL_SetRenderDrawColor(renderer, 0xa2, 0xa2, 0xa2, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}

void draw_box_hover(SDL_Renderer *renderer, SDL_FRect *rect) {
	draw_box(renderer, rect);

	SDL_SetRenderDrawColor(renderer, 0xa6, 0xad, 0xdb, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}

void draw_box_active(SDL_Renderer *renderer, SDL_FRect *rect) {
	draw_box(renderer, rect);

	SDL_SetRenderDrawColor(renderer, 0x74, 0x74, 0x74, 0xFF);
	SDL_RenderFillRect(renderer, RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}
