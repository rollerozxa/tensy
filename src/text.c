#include "text.h"
#include "font.h"
#include <string.h>

void text_draw(const char *text, float x, float y, float scale) {
	for (size_t i = 0; text[i] != '\0'; i++)
		font_draw_char(text[i], x + i * GLYPH_WIDTH * scale, y, scale);
}

void text_draw_centered(const char *text, SDL_FRect *rect, float scale) {
	SDL_FRect text_rect = text_calc_rect(text, scale);

	text_draw(text, rect->x + (rect->w - text_rect.w) / 2, rect->y + (rect->h - text_rect.h) / 2, scale);
}

void text_draw_shadow(const char *text, float x, float y, float scale) {
	for (size_t i = 0; text[i] != '\0'; i++)
		font_draw_char_shadow(text[i], x + i * GLYPH_WIDTH * scale, y, scale);
}

void text_draw_shadow_centered(const char *text, SDL_FRect *rect, float scale) {
	SDL_FRect text_rect = text_calc_rect(text, scale);

	text_draw_shadow(text, rect->x + (rect->w - text_rect.w) / 2, rect->y + (rect->h - text_rect.h) / 2, scale);
}

SDL_FRect text_calc_rect(const char *text, float scale) {
	SDL_FRect rect = {
		0, 0,
		strlen(text) * GLYPH_WIDTH * scale,
		GLYPH_HEIGHT * scale
	};

	return rect;
}
