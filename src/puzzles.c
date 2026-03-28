#include "puzzles.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// asset loader stuff
#include "media/assetloader.h"

// Helper to read whole SDL_IOStream into a NUL-terminated buffer.
static char *read_all_from_io(SDL_IOStream *io, size_t *out_size) {
	Sint64 size = SDL_SeekIO(io, 0, SDL_IO_SEEK_END);

	SDL_SeekIO(io, 0, SDL_IO_SEEK_SET);
	char *buf = malloc(size + 1);

	size_t got = SDL_ReadIO(io, buf, size);
	buf[got] = '\0';

	if (out_size)
		*out_size = got;

	return buf;
}

// Simple line reader: returns pointer to next line and NUL-terminates it in-place.
static char *next_line(char **p) {
	char *start = *p;
	char *nl = strchr(start, '\n');
	if (nl) {
		*nl = '\0';

		// trim CR if present
		if (nl > start && *(nl - 1) == '\r')
			*(nl - 1) = '\0';

		*p = nl + 1;
	} else {
		*p = start + strlen(start);
	}

	return start;
}

#if EMBEDDED_DATA
#include <data/lvl_0_puz.h>
#include <data/lvl_1_puz.h>
#include <data/lvl_2_puz.h>

static unsigned char *puzzle_files[] = {
	lvl_0_puz,
	lvl_1_puz,
	lvl_2_puz,
};
static unsigned int puzzle_file_sizes[] = {
	sizeof(lvl_0_puz),
	sizeof(lvl_1_puz),
	sizeof(lvl_2_puz),
};
#endif

int puzzle_get_count(void) {
	static int levels = 0;

#ifdef ASSETLOADER_LOOSE
	if (levels == 0) {
		SDL_IOStream *dot_count = SDL_IOFromFile(PREFIX "puzzles/_lvl_count", "rb");
		char tmp[8] = {0};
		SDL_ReadIO(dot_count, tmp, sizeof(tmp) - 1);
		SDL_CloseIO(dot_count);
		levels = SDL_atoi(tmp);
	}
#else
	levels = SDL_arraysize(puzzle_files);
#endif

	return levels;
}

char *puzzle_get_file(int index) {

#ifdef ASSETLOADER_LOOSE
	char path[256];
	snprintf(path, sizeof(path), PREFIX "puzzles/lvl_%d.puz", index);
	SDL_IOStream *io = SDL_IOFromFile(path, "rb");
#else
	SDL_IOStream *io = SDL_IOFromMem(puzzle_files[index], puzzle_file_sizes[index]);
#endif

	char *buf = read_all_from_io(io, NULL);
	SDL_CloseIO(io);
	return buf;
}

Puzzle *puzs = NULL;

void puzzles_load(void) {

	int levels = puzzle_get_count();

	puzs = (Puzzle *)calloc(levels, sizeof(Puzzle));

	for (int i = 0; i < levels; ++i) {
		char *buf = puzzle_get_file(i);

		int width = atoi(next_line(&buf));
		int height = atoi(next_line(&buf));
		float boardsize = (float)atoi(next_line(&buf)) / 10;

		char **board = malloc(sizeof(char*) * height);
		for (int r = 0; r < height; ++r) {
			char *ln = next_line(&buf);
			char *row = calloc(width + 1, 1);
			int copylen = (int)SDL_min(strlen(ln), (size_t)width);
			SDL_memcpy(row, ln, copylen);
			board[r] = row;
		}

		puzs[i].board = board;
		puzs[i].boardsize = boardsize;
		puzs[i].width = width;
		puzs[i].height = height;
	}
}

Puzzle *puzzles(void) {
	return puzs;
}
