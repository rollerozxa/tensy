#include "font.h"
#include "media/font_bitmap.h"

static SDL_Texture *font_tex;

static SDL_Texture *font_load(SDL_Renderer *renderer) {
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

SDL_Color font_get_color(void) {
	return font_color;
}

void font_set_color(SDL_Color clr) {
	font_color = clr;
	SDL_SetTextureColorMod(font_tex, clr.r, clr.g, clr.b);
}

void font_draw_char(SDL_Renderer *renderer, unsigned char character, float cx, float cy, float scale) {
	SDL_Point cell = {
		(character-32) % (FONT_WIDTH/GLYPH_WIDTH),
		(character-32) / (FONT_WIDTH/GLYPH_WIDTH)
	};

	if (font_tex == NULL)
		font_tex = font_load(renderer);

	SDL_FRect srcrect = {cell.x * GLYPH_WIDTH, cell.y * GLYPH_HEIGHT, GLYPH_WIDTH, GLYPH_HEIGHT};
	SDL_FRect dstrect = {cx, cy, GLYPH_WIDTH * scale, GLYPH_HEIGHT * scale};

	SDL_RenderTexture(renderer, font_tex, &srcrect, &dstrect);
}

void font_draw_char_shadow(SDL_Renderer *renderer, unsigned char character, float cx, float cy, float scale) {
	SDL_Color temp = font_get_color();

	// TODO: Make this more better (this is just to make toasts fade out)
	unsigned char alpha;
	SDL_GetRenderDrawColor(renderer, NULL, NULL, NULL, &alpha);
	SDL_SetTextureAlphaMod(font_tex, alpha);

	font_set_color((SDL_Color){0, 0, 0});
	font_draw_char(renderer, character, cx + scale, cy + scale, scale);
	font_set_color(temp);
	font_draw_char(renderer, character, cx, cy, scale);

	SDL_SetTextureAlphaMod(font_tex, SDL_ALPHA_OPAQUE);
}

// (Text drawing functions have been moved to src/text.c)
