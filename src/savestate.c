#include "savestate.h"
#include "board.h"
#include "consts.h"
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
	WRITE_INT(score);
	WRITE_INT(gamemode);
	WRITE_INT(time_left);
	WRITE_INT(total_time);
	WRITE_INT(board.w);
	WRITE_INT(board.h);
	WRITE_FLOAT(board.scale);
	WRITE_FLOAT(board.cell_size);
	for (int x = 0; x < board.w; x++) {
		for (int y = 0; y < board.h; y++) {
			// MSB is used to store whether a cell has been removed
			char packed = board.p[x][y].number + (board.p[x][y].removed << 7);
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

	READ_INT(score);
	READ_INT(gamemode);
	READ_INT(time_left);
	READ_INT(total_time);
	READ_INT(board.w);
	READ_INT(board.h);
	READ_FLOAT(board.scale);
	READ_FLOAT(board.cell_size);
	board_change_scale(&board, board.scale);
	board_reset(&board);
	for (int x = 0; x < board.w; x++) {
		for (int y = 0; y < board.h; y++) {
			char packed;
			READ_CHAR(packed);
			// Unpack cell removed value from the MSB
			board.p[x][y].number = packed & 0x7F;
			board.p[x][y].removed = (packed & 0x80) != 0;
		}
	}
	fclose(fp);

	return true;
}
