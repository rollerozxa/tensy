#include "savestate.h"
#include "board.h"
#include "consts.h"
#include "gamestate.h"
#include "scenes/game.h"
#include <stdio.h>

#define WRITE_INT(data)   fwrite(&data, sizeof(int), 1, fp)
#define WRITE_FLOAT(data) fwrite(&data, sizeof(float), 1, fp)
#define WRITE_CHAR(data)  fwrite(&data, sizeof(char), 1, fp)
#define WRITE_BOOL(data)  fwrite(&data, sizeof(bool), 1, fp)

#define READ_INT(data)    fread(&data, sizeof(int), 1, fp)
#define READ_FLOAT(data)  fread(&data, sizeof(float), 1, fp)
#define READ_CHAR(data)   fread(&data, sizeof(char), 1, fp)
#define READ_BOOL(data)   fread(&data, sizeof(bool), 1, fp)

const static char filever = 1;

static char statesave_file[512];

static char *get_filepath(void) {
	if (statesave_file[0] == '\0') {
		strncpy(statesave_file, SDL_GetPrefPath(APP_ORG, APP_NAME), 511);
		strncat(statesave_file, "savestate.bin", 511);
	}

	return statesave_file;
}

bool savestate_exists(void) {
    FILE *file = fopen(get_filepath(), "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

bool savestate_save(void) {
	FILE *fp = fopen(get_filepath(), "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);
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
