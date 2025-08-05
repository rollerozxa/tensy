#include "draw.h"
#include "consts.h"
#include "media/textures.h"

bool draw_line(float x1, float y1, float x2, float y2) {
	return SDL_RenderLine(renderer, x1, y1, x2, y2);
}

bool draw_rect(const SDL_FRect *rect) {
	return SDL_RenderRect(renderer,rect);
}

bool draw_fill_rect(const SDL_FRect *rect) {
	return SDL_RenderFillRect(renderer,rect);
}

bool draw_texture(int texture_id, const SDL_FRect *srcrect, const SDL_FRect *dstrect) {
	return SDL_RenderTexture(renderer, textures_get(texture_id), srcrect, dstrect);
}

void draw_set_color(unsigned int color) {
	SDL_SetRenderDrawColor(renderer,
		(color >> 16) & 0xFF,
		(color >> 8)  & 0xFF,
		color & 0xFF,
		SDL_ALPHA_OPAQUE);
}

void draw_set_color_alpha(unsigned int color) {
	SDL_SetRenderDrawColor(renderer,
		(color >> 24) & 0xFF,
		(color >> 16) & 0xFF,
		(color >> 8)  & 0xFF,
		color & 0xFF);
}

void draw_box(SDL_FRect *rect) {
	draw_set_color(0x131313);
	draw_fill_rect(&RECT(rect->x, rect->y, rect->w, rect->h));

	draw_set_color(0xFFFFFF);
	draw_fill_rect(&RECT(rect->x+1, rect->y+1, rect->w-2, rect->h-2));

	draw_set_color(0xC6C6C6);
	draw_fill_rect(&RECT(rect->x+1, rect->y+rect->h-2, 1, 1));
	draw_fill_rect(&RECT(rect->x+rect->w-2, rect->y+1, 1, 1));

	draw_set_color(0x585658);
	draw_fill_rect(&RECT(rect->x+2, rect->y+2, rect->w-3, rect->h-3));

	draw_set_color(0xA2A2A2);
	draw_fill_rect(&RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}

void draw_box_hover(SDL_FRect *rect) {
	draw_box(rect);

	draw_set_color(0xA6ADDB);
	draw_fill_rect(&RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}

void draw_box_active(SDL_FRect *rect) {
	draw_box(rect);

	draw_set_color(0x747474);
	draw_fill_rect(&RECT(rect->x+2, rect->y+2, rect->w-4, rect->h-4));
}

void draw_translucent_overlay(void) {
	draw_set_color_alpha(0x000000d0);
	draw_fill_rect(&RECT(0,0,NATIVE_WIDTH,NATIVE_HEIGHT));
}

SDL_FRect draw_centered_fill_rect(SDL_FPoint dimensions) {
	SDL_FRect rect = {
		0, 0,
		dimensions.x,
		dimensions.y,
	};
	rect.x = (NATIVE_WIDTH - rect.w) / 2;
	rect.y = (NATIVE_HEIGHT - rect.h) / 2;

	draw_set_color(0x102a63);
	draw_fill_rect(&rect);

	return rect;
}
