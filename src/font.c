#include "font.h"
#include "draw.h"
#include "media/font_bitmap.h"

static SDL_Texture *font_tex;
#define GLYPH_PADDING 1

static SDL_Texture *font_load(void) {
	int glyphs_x = FONT_WIDTH  / GLYPH_WIDTH;
	int glyphs_y = FONT_HEIGHT / GLYPH_HEIGHT;

	int padded_width  = FONT_WIDTH  + (glyphs_x * GLYPH_PADDING);
	int padded_height = FONT_HEIGHT + (glyphs_y * GLYPH_PADDING);

	SDL_Surface *surface = SDL_CreateSurface(
		padded_width, padded_height,
		SDL_GetPixelFormatForMasks(32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

	Uint32 white = SDL_MapSurfaceRGB(surface, 255, 255, 255);
	Uint32 black = SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0);

	int pitch = surface->pitch / 4;

	for (int gy = 0; gy < glyphs_y; ++gy) {
	for (int gx = 0; gx < glyphs_x; ++gx) {

		int dst_x0 = gx * (GLYPH_WIDTH + GLYPH_PADDING);
		int dst_y0 = gy * (GLYPH_HEIGHT + GLYPH_PADDING);

		for (int y = 0; y < GLYPH_HEIGHT; ++y) {
		for (int x = 0; x < GLYPH_WIDTH; ++x) {
			int src_index = (gy * GLYPH_HEIGHT + y) * FONT_WIDTH + (gx * GLYPH_WIDTH + x);
			int dst_index = (dst_y0 + y) * pitch + (dst_x0 + x);

			Uint32 color = font_bitmap[gy * GLYPH_HEIGHT + y][gx * GLYPH_WIDTH + x] ? white : black;
			((Uint32 *)surface->pixels)[dst_index] = color;
		}}
	}}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	SDL_DestroySurface(surface);

	return texture;
}

static SDL_Color font_color;

SDL_Color font_get_color(void) {
	return font_color;
}

void font_set_color(SDL_Color clr) {
	font_color = clr;
	SDL_SetTextureColorMod(font_tex, clr.r, clr.g, clr.b);
}

void font_draw_char(unsigned char character, float cx, float cy, float scale) {
	SDL_Point cell = {
		(character-32) % (FONT_WIDTH/GLYPH_WIDTH),
		(character-32) / (FONT_WIDTH/GLYPH_WIDTH)
	};

	if (font_tex == NULL)
		font_tex = font_load();

	SDL_FRect srcrect = {cell.x * (GLYPH_WIDTH + GLYPH_PADDING), cell.y * (GLYPH_HEIGHT + GLYPH_PADDING), GLYPH_WIDTH, GLYPH_HEIGHT};
	SDL_FRect dstrect = {cx, cy, GLYPH_WIDTH * scale, GLYPH_HEIGHT * scale};

	SDL_RenderTexture(renderer, font_tex, &srcrect, &dstrect);
}

void font_draw_char_shadow(unsigned char character, float cx, float cy, float scale) {
	SDL_Color temp = font_get_color();

	// TODO: Make this more better (this is just to make toasts fade out)
	unsigned char alpha;
	SDL_GetRenderDrawColor(renderer, NULL, NULL, NULL, &alpha);
	SDL_SetTextureAlphaMod(font_tex, alpha);

	font_set_color((SDL_Color){0, 0, 0});
	font_draw_char(character, cx + scale, cy + scale, scale);
	font_set_color(temp);
	font_draw_char(character, cx, cy, scale);

	SDL_SetTextureAlphaMod(font_tex, SDL_ALPHA_OPAQUE);
}

// (Text drawing functions have been moved to src/text.c)
