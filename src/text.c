#include "text.h"
#include "font.h"

void text_draw(SDL_Renderer *renderer, const char *text, float x, float y, float scale) {
	for (size_t i = 0; text[i] != '\0'; i++)
		font_draw_char(renderer, text[i], x + i * GLYPH_WIDTH * scale, y, scale);
}

void text_draw_centered(SDL_Renderer *renderer, const char *text, SDL_FRect *rect, float scale) {
	SDL_FRect text_rect = text_calc_rect(text, scale);

	text_draw(renderer, text, rect->x + (rect->w - text_rect.w) / 2, rect->y + (rect->h - text_rect.h) / 2, scale);
}

void text_draw_shadow(SDL_Renderer *renderer, const char *text, float x, float y, float scale) {
	for (size_t i = 0; text[i] != '\0'; i++)
		font_draw_char_shadow(renderer, text[i], x + i * GLYPH_WIDTH * scale, y, scale);
}

void text_draw_shadow_centered(SDL_Renderer *renderer, const char *text, SDL_FRect *rect, float scale) {
	SDL_FRect text_rect = text_calc_rect(text, scale);

	text_draw_shadow(renderer, text, rect->x + (rect->w - text_rect.w) / 2, rect->y + (rect->h - text_rect.h) / 2, scale);
}

SDL_FRect text_calc_rect(const char *text, float scale) {
	SDL_FRect rect = {
		0, 0,
		strlen(text) * GLYPH_WIDTH * scale,
		GLYPH_HEIGHT * scale
	};

	return rect;
}
