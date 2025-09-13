#include "font.h"

#include <stddef.h>

#include "draw.h"
#include "media/font_bitmap.h"

static SDL_Texture *font_tex;

// So this code has seen some increase in complexity over the span of the project.
// Points of note:
// - Switching to pixelart scalemode (which uses a slightly smarter scaling algorithm than nearest
//   neighbour) made neighbouring glyph edges bleed into eachother, so glyphs now have padding.
// - Vita doesn't support pixelart scaling so to make the font not look crusty, we use linear scaling
//   and then also scale it up by some big amount to not make it blurry.

#ifdef SDL_PLATFORM_VITA
	#define FONT_SCALE 8
	#define FONT_SCALEMODE SDL_SCALEMODE_LINEAR
#else
	#define FONT_SCALE 1
	#define FONT_SCALEMODE SDL_SCALEMODE_PIXELART
#endif
#define GLYPH_PADDING 1

static SDL_Texture *font_load(void) {
	int glyphs_x = FONT_WIDTH  / GLYPH_WIDTH;
	int glyphs_y = FONT_HEIGHT / GLYPH_HEIGHT;

	int cell_w = GLYPH_WIDTH  * FONT_SCALE; // scaled glyph width in atlas
	int cell_h = GLYPH_HEIGHT * FONT_SCALE; // scaled glyph height in atlas

	// total atlas size: glyph columns * scaled cell width + padding per cell (unscaled)
	int padded_width  = glyphs_x * cell_w + glyphs_x * GLYPH_PADDING;
	int padded_height = glyphs_y * cell_h + glyphs_y * GLYPH_PADDING;

	SDL_Log("Rendering font at %dx%d", padded_width, padded_height);

	SDL_Surface *surface = SDL_CreateSurface(
		padded_width, padded_height,
		SDL_GetPixelFormatForMasks(32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

	Uint32 white = SDL_MapSurfaceRGB(surface, 255, 255, 255);
	Uint32 black = SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0);

	int pitch = surface->pitch / 4;
	Uint32 *pixels = (Uint32 *)surface->pixels;

	for (int gy = 0; gy < glyphs_y; ++gy) {
	for (int gx = 0; gx < glyphs_x; ++gx) {

		// dest origin: glyph index * (scaled_glyph + unscaled padding)
		int dst_x0 = gx * (cell_w + GLYPH_PADDING);
		int dst_y0 = gy * (cell_h + GLYPH_PADDING);

		for (int y = 0; y < GLYPH_HEIGHT; ++y) {
		for (int x = 0; x < GLYPH_WIDTH; ++x) {
			Uint32 color = font_bitmap[gy * GLYPH_HEIGHT + y][gx * GLYPH_WIDTH + x] ? white : black;

			int dst_base_x = dst_x0 + x * FONT_SCALE;
			int dst_base_y = dst_y0 + y * FONT_SCALE;

#if FONT_SCALE > 1
			// scale up to a FONT_SCALE x FONT_SCALE block (nearest neighbour)
			for (int sy = 0; sy < FONT_SCALE; ++sy)
				for (int sx = 0; sx < FONT_SCALE; ++sx)
					pixels[((dst_base_y + sy) * pitch + dst_base_x) + sx] = color;
#else
			pixels[dst_base_y * pitch + dst_base_x] = color;
#endif
		}}
	}}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(texture, FONT_SCALEMODE);
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

	int src_w = GLYPH_WIDTH * FONT_SCALE;
	int src_h = GLYPH_HEIGHT * FONT_SCALE;

	int src_pitch_x = src_w + GLYPH_PADDING;
	int src_pitch_y = src_h + GLYPH_PADDING;

	SDL_FRect srcrect = {
		cell.x * src_pitch_x,
		cell.y * src_pitch_y,
		src_w, src_h
	};

	SDL_FRect dstrect = {
		cx, cy,
		GLYPH_WIDTH * scale,
		GLYPH_HEIGHT * scale
	};
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
