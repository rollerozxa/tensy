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

#define COLOR_COUNT 9

static SDL_Color custom_colors[COLOR_COUNT];
static bool custom_colors_loaded = false;

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

void color_load_custom_numbers() {
	char path[512] = {0};
	fileio_pref_path(path, sizeof(path), "number_colors.txt");

	FILE *f = fopen(path, "r");

	if (!f) return;

	char data[6 * 9 + 8]; // 9 colours of 6 hex digits + colons
	fread(data, 1, sizeof(data), f);

	fclose(f);

	for (int i = 0; i < COLOR_COUNT; i++) {
		char hex[7] = {0};
		strncpy(hex, &data[i * 7], 6);
		custom_colors[i] = parse_hex_color(hex);
	}
	custom_colors_loaded = true;
}

/*
 * Get a nice unique colour for each number
 */
SDL_Color color_numbers(int num) {
	if (custom_colors_loaded && num >= 1 && num <= COLOR_COUNT)
		return custom_colors[num - 1];

	switch (num) {
		case 1: return (SDL_Color){0xFF, 0x98, 0x8F};
		case 2: return (SDL_Color){0xFF, 0xB1, 0x69};
		case 3: return (SDL_Color){0xFF, 0xF0, 0x69};
		case 4: return (SDL_Color){0xA0, 0xFF, 0x69};
		case 5: return (SDL_Color){0x69, 0xFF, 0x73};
		case 6: return (SDL_Color){0x69, 0xFF, 0xD2};
		case 7: return (SDL_Color){0x69, 0xBB, 0xFF};
		case 8: return (SDL_Color){0x9D, 0x7D, 0xFF};
		case 9: return (SDL_Color){0xFF, 0x7D, 0xE7};
	}

	return CLR_WHITE;
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
