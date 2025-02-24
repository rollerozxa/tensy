#include "font.h"
#include "font_bitmap.h"

SDL_Texture *font_tex;

SDL_Texture *load_font(SDL_Renderer *renderer) {
	SDL_Surface *surface = SDL_CreateSurface(FONT_WIDTH, FONT_HEIGHT,
		SDL_GetPixelFormatForMasks(32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

	Uint32 white = SDL_MapSurfaceRGB(surface, 255, 255, 255);
	Uint32 black = SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0);

	for (int y = 0; y < FONT_HEIGHT; ++y) {
		for (int x = 0; x < FONT_WIDTH; ++x) {
			Uint32 color = font_bitmap[y][x] ? white : black;
			((Uint32 *)surface->pixels)[y * FONT_WIDTH + x] = color;
		}
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	SDL_DestroySurface(surface);

	return texture;
}

static SDL_Color font_color;

SDL_Color get_font_color(void) {
	return font_color;
}

void set_font_color(SDL_Color clr) {
	font_color = clr;
	SDL_SetTextureColorMod(font_tex, clr.r, clr.g, clr.b);
}

void draw_char(SDL_Renderer *renderer, unsigned char character, float cx, float cy, float scale) {
	SDL_Point cell = {
		(character-32) % (FONT_WIDTH/GLYPH_WIDTH),
		(character-32) / (FONT_WIDTH/GLYPH_WIDTH)
	};

	if (font_tex == NULL)
		font_tex = load_font(renderer);

	SDL_FRect srcrect = {cell.x * GLYPH_WIDTH, cell.y * GLYPH_HEIGHT, GLYPH_WIDTH, GLYPH_HEIGHT};
	SDL_FRect dstrect = {cx, cy, GLYPH_WIDTH * scale, GLYPH_HEIGHT * scale};

	SDL_RenderTexture(renderer, font_tex, &srcrect, &dstrect);
}

void draw_char_shadow(SDL_Renderer *renderer, unsigned char character, float cx, float cy, float scale) {
	SDL_Color temp = get_font_color();

	set_font_color((SDL_Color){0, 0, 0});
	draw_char(renderer, character, cx + scale, cy + scale, scale);
	set_font_color(temp);
	draw_char(renderer, character, cx, cy, scale);
}

void draw_text(SDL_Renderer *renderer, const char *text, float x, float y, float scale) {
	for (size_t i = 0; text[i] != '\0'; i++)
		draw_char(renderer, text[i], x + i * GLYPH_WIDTH * scale, y, scale);
}

void draw_text_shadow(SDL_Renderer *renderer, const char *text, float x, float y, float scale) {
	for (size_t i = 0; text[i] != '\0'; i++)
		draw_char_shadow(renderer, text[i], x + i * GLYPH_WIDTH * scale, y, scale);
}

void draw_text_shadow_centered(SDL_Renderer *renderer, const char *text, SDL_FRect *rect, float scale) {
	SDL_FRect text_rect = calculate_text_rect(text, scale);

	draw_text_shadow(renderer, text, rect->x + (rect->w - text_rect.w) / 2, rect->y + (rect->h - text_rect.h) / 2, scale);
}

SDL_FRect calculate_text_rect(const char *text, float scale) {
	SDL_FRect rect = {
		0, 0,
		strlen(text) * GLYPH_WIDTH * scale,
		GLYPH_HEIGHT * scale
	};

	return rect;
}
