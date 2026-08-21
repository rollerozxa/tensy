#include "savestate.h"
#include "board.h"
#include "fileio.h"
#include "gamestate.h"
#include <stdio.h>
#include <unistd.h>

static const uint8_t filever = 2;

static char statesave_file[512];

bool savestate_exists(void) {
	fileio_pref_path(statesave_file, sizeof(statesave_file), "savestate.bin");

	SDL_IOStream *io = SDL_IOFromFile(statesave_file, "rb");
	if (io) {
		uint8_t tmp;
		READ_BYTE(tmp);
		if (tmp != filever)
			return false; // wrong version, pretend it doesn't exist

		SDL_CloseIO(io);
		return true;
	}

	return false;
}

bool savestate_delete(void) {
	SDL_IOStream *io = SDL_IOFromFile(statesave_file, "rb");
	if (io) {
		// Make sure we grabbed a file and not the user's home folder or something equally horrifying
		SDL_CloseIO(io);
		SDL_RemovePath(statesave_file);
		return true;
	}

	return false;
}

bool savestate_save(void) {
	SDL_IOStream *io = SDL_IOFromFile(statesave_file, "wb");
	if (!io)
		return false;

	WRITE_BYTE(filever);
	WRITE_LONG(game.identifier);
	WRITE_UINT(game.score);
	WRITE_INT(game.mode);
	WRITE_FLOAT(game.time_left);
	WRITE_FLOAT(game.total_time);
	WRITE_INT(game.board.w);
	WRITE_INT(game.board.h);
	WRITE_FLOAT(game.board.scale);
	WRITE_FLOAT(game.board.cell_size);
	WRITE_BOOL(game.board.physics);
	for (int x = 0; x < game.board.w; x++) {
		for (int y = 0; y < game.board.h; y++) {
			// MSB is used to store whether a cell has been removed
			uint8_t packed = game.board.p[x][y].number
					+ (game.board.p[x][y].removed << 7);
			WRITE_BYTE(packed);
		}
	}

	for (int i = 0; i < 9; i++)
		WRITE_SHORT(game.number_stats[i]);

	SDL_CloseIO(io);

	game.dirty = false;

	return true;
}

bool savestate_load(void) {
	SDL_IOStream *io = SDL_IOFromFile(statesave_file, "rb");
	if (!io)
		return false;

	uint8_t tmp;
	READ_BYTE(tmp);
	if (tmp != filever)
		return false; // uhh

	READ_LONG(game.identifier);
	READ_UINT(game.score);
	int mode;
	READ_INT(mode);
	game.mode = (enum GameMode)mode;
	READ_FLOAT(game.time_left);
	READ_FLOAT(game.total_time);
	READ_INT(game.board.w);
	READ_INT(game.board.h);
	READ_FLOAT(game.board.scale);
	READ_FLOAT(game.board.cell_size);
	READ_BOOL(game.board.physics);
	board_change_scale(&game.board, game.board.scale);
	board_reset(&game.board);
	for (int x = 0; x < game.board.w; x++) {
		for (int y = 0; y < game.board.h; y++) {
			uint8_t packed;
			READ_BYTE(packed);
			// Unpack cell removed value from the MSB
			game.board.p[x][y].number = packed & 0x7F;
			game.board.p[x][y].removed = (packed & 0x80) != 0;
		}
	}
	for (int i = 0; i < 9; i++) {
		uint16_t stat;
		READ_SHORT(stat);
		game.number_stats[i] = stat;
	}

	SDL_CloseIO(io);

	return true;
}

uint64_t savestate_read_identifier(void) {
	SDL_IOStream *io = SDL_IOFromFile(statesave_file, "rb");
	if (!io)
		return false;

	uint8_t tmp;
	READ_BYTE(tmp);
	if (tmp != filever)
		return false; // uhh

	uint64_t identifier;
	READ_LONG(identifier);
	return identifier;
}
