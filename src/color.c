#include "color.h"
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * This source file had to be renamed to color.c in order to prevent Tensy
 * from falling under legislation of the 2023 Online Safety Act.
 *
 * (I kid...)
 */

static int hex_to_int(char c) {
	if ('0' <= c && c <= '9') return c - '0';
	if ('a' <= c && c <= 'f') return c - 'a' + 10;
	if ('A' <= c && c <= 'F') return c - 'A' + 10;
	return 0;
}

static SDL_Color parse_hex_color(const char *hex) {
	if (strnlen(hex, 7) != 6) return CLR_WHITE;

	return (SDL_Color){
		(hex_to_int(hex[0]) << 4) | hex_to_int(hex[1]),
		(hex_to_int(hex[2]) << 4) | hex_to_int(hex[3]),
		(hex_to_int(hex[4]) << 4) | hex_to_int(hex[5]),
		255};
}

unsigned int parse_hex_color_bitpack(const char *hex) {
	SDL_Color c = parse_hex_color(hex);
	return color_sdl_to_bitpack(c);
}

void color_to_hex6(unsigned int color, char *out) {
	SDL_Color c = color_bitpack_to_sdl(color);
	snprintf(out, 7, "%02X%02X%02X", c.r, c.g, c.b);
	out[6] = '\0';
}

unsigned int color_sdl_to_bitpack(SDL_Color color) {
	return (color.r << 16)
		 + (color.g << 8)
		 + color.b;
}

SDL_Color color_bitpack_to_sdl(unsigned int color) {
	return (SDL_Color){
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF,
		255};
}

SDL_Color HSVtoRGB(double h, double s, double v) {
	float r, g, b;

	int i = (int)(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}

	return (SDL_Color){
		(unsigned char)(r * 255),
		(unsigned char)(g * 255),
		(unsigned char)(b * 255),
		255
	};
}

// Number color stuff

#define COLOR_COUNT 9

static SDL_Color number_colors[COLOR_COUNT];
static const char default_colors[] = "FF988F:FFB169:FFF069:A0FF69:69FF73:69FFD2:69BBFF:9D7DFF:FF7DE7";

void color_numbers_load(char *text) {
	for (int i = 0; i < COLOR_COUNT; i++) {
		char hex[7] = {0};
		memcpy(hex, &text[i * 7], 6);
		number_colors[i] = parse_hex_color(hex);
	}
}

void color_load_custom_numbers(void) {
	char path[512] = {0};
	fileio_pref_path(path, sizeof(path), "number_colors.txt");

	FILE *f = fopen(path, "r");

	if (!f) {
		color_numbers_load((char *)default_colors);
		return;
	}

	char data[6 * 9 + 8]; // 9 colours of 6 hex digits + colons
	fread(data, 1, sizeof(data), f);
	fclose(f);

	color_numbers_load(data);
}

void color_save_custom_numbers(void) {
	char path[512] = {0};
	fileio_pref_path(path, sizeof(path), "number_colors.txt");

	FILE *f = fopen(path, "w");
	if (!f) return;

	char buf[8] = {0};
	for (int i = 1; i <= COLOR_COUNT; i++) {
		SDL_Color c = color_numbers(i);
		unsigned int bp = color_sdl_to_bitpack(c);
		color_to_hex6(bp, buf);
		fputs(buf, f);
		if (i != COLOR_COUNT)
			fputc(':', f);
	}

	fclose(f);
}

void color_numbers_reset(void) {
	color_numbers_load((char *)default_colors);
}

/*
 * Get a nice unique colour for each number
 */
SDL_Color color_numbers(int num) {
	if (num >= 1 && num <= COLOR_COUNT)
		return number_colors[num - 1];

	return CLR_WHITE;
}

void color_set_number_color(int num, unsigned int color) {
	if (num < 1 || num > COLOR_COUNT)
		return;

	number_colors[num - 1] = color_bitpack_to_sdl(color);
}
