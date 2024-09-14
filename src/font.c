#include "font.h"
#include "font_bitmap.h"
#include "consts.h"

SDL_Texture* font_tex;

SDL_Texture* load_font(SDL_Renderer *renderer) {
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

#define FGRID_WIDTH (FONT_WIDTH/GLYPH_WIDTH)
#define FGRID_HEIGHT (FONT_HEIGHT/GLYPH_HEIGHT)

void set_font_color(SDL_Color clr) {
	SDL_SetTextureColorMod(font_tex, clr.r, clr.g, clr.b);
}

void draw_char(SDL_Renderer *renderer, unsigned char character, int cx, int cy, int scale) {
	int cell_x = (character-32) % FGRID_WIDTH;
	int cell_y = (character-32) / FGRID_WIDTH;

	if (font_tex == NULL) {
		font_tex = load_font(renderer);
	}

	SDL_RenderTexture(
		renderer, font_tex,
		RECT(cell_x*GLYPH_WIDTH, cell_y*GLYPH_HEIGHT, GLYPH_WIDTH, GLYPH_HEIGHT),
		RECT(cx, cy, GLYPH_WIDTH*scale, GLYPH_HEIGHT*scale)
	);
}

void draw_text(SDL_Renderer *renderer, const char* text, int x, int y, int scale) {
	for (size_t i = 0; text[i] != '\0'; i++) {
		draw_char(renderer, text[i], x+i*GLYPH_WIDTH*scale, y*scale, scale);
	}
}