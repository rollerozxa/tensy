#include "savestate.h"
#include "board.h"
#include "consts.h"
#include "gamestate.h"
#include "fileio.h"
#include <stdio.h>
#include <unistd.h>

const static char filever = 2;

static char statesave_file[512];

static char *get_filepath(void) {
	if (statesave_file[0] == '\0') {
		strncpy(statesave_file, SDL_GetPrefPath(APP_ORG, APP_NAME), 511);
		strncat(statesave_file, "savestate.bin", 511);
	}

	return statesave_file;
}

bool savestate_exists(void) {
	FILE *fp = fopen(get_filepath(), "r");
	if (fp) {
		char tmp;
		READ_CHAR(tmp);
		if (tmp != filever)
			return false; // wrong version, pretend it doesn't exist

		fclose(fp);
		return true;
	}

	return false;
}

bool savestate_delete(void) {
	FILE *file = fopen(get_filepath(), "r");
	if (file) {
		// Make sure we grabbed a file and not the user's home folder or something equally horrifying
		fclose(file);
		unlink(get_filepath());
		return true;
	}

	return false;
}

bool savestate_save(void) {
	FILE *fp = fopen(get_filepath(), "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);
	WRITE_LONG(game.identifier);
	WRITE_INT(game.score);
	WRITE_INT(game.mode);
	WRITE_INT(game.time_left);
	WRITE_INT(game.total_time);
	WRITE_INT(game.board.w);
	WRITE_INT(game.board.h);
	WRITE_FLOAT(game.board.scale);
	WRITE_FLOAT(game.board.cell_size);
	WRITE_BOOL(game.board.physics);
	for (int x = 0; x < game.board.w; x++) {
		for (int y = 0; y < game.board.h; y++) {
			// MSB is used to store whether a cell has been removed
			char packed = game.board.p[x][y].number
					+ (game.board.p[x][y].removed << 7);
			WRITE_CHAR(packed);
		}
	}

	fclose(fp);

	game.dirty = false;

	return true;
}

bool savestate_load(void) {
	FILE *fp = fopen(get_filepath(), "rb");
	if (!fp)
		return false;

	char tmp;
	READ_CHAR(tmp);
	if (tmp != filever)
		return false; // uhh

	READ_LONG(game.identifier);
	READ_INT(game.score);
	READ_INT(game.mode);
	READ_INT(game.time_left);
	READ_INT(game.total_time);
	READ_INT(game.board.w);
	READ_INT(game.board.h);
	READ_FLOAT(game.board.scale);
	READ_FLOAT(game.board.cell_size);
	READ_BOOL(game.board.physics);
	board_change_scale(&game.board, game.board.scale);
	board_reset(&game.board);
	for (int x = 0; x < game.board.w; x++) {
		for (int y = 0; y < game.board.h; y++) {
			char packed;
			READ_CHAR(packed);
			// Unpack cell removed value from the MSB
			game.board.p[x][y].number = packed & 0x7F;
			game.board.p[x][y].removed = (packed & 0x80) != 0;
		}
	}
	fclose(fp);

	return true;
}

uint64_t savestate_read_identifier(void) {
	FILE *fp = fopen(get_filepath(), "rb");
	if (!fp)
		return false;

	char tmp;
	READ_CHAR(tmp);
	if (tmp != filever)
		return false; // uhh

	uint64_t identifier;
	fread(&identifier, sizeof(uint64_t), 1, fp);
	return identifier;
}
