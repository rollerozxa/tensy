#include "gamesettings.h"
#include "consts.h"
#include "fileio.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static Settings settings_data = {
	false,
	false,
};

Settings *settings(void) {
	return &settings_data;
}

const static char filever = 1;

static char settings_file[512];

static char *get_filepath(void) {
	if (settings_file[0] == '\0') {
		strncpy(settings_file, SDL_GetPrefPath(APP_ORG, APP_NAME), 511);
		strncat(settings_file, "settings.dat", 511);
	}

	return settings_file;
}

#define READ_FLAG(key, flags, flag) \
	settings_data.key = flags & (1ULL << flag);

bool settings_load(void) {
	FILE *fp = fopen(get_filepath(), "rb");
	if (!fp)
		return false;

	char tmp;
	READ_CHAR(tmp);
	if (tmp != filever)
		return false; // uhh

	int flags;
	READ_INT(flags);
	READ_FLAG(coloured_numbers, flags, 0);
	READ_FLAG(secret_five, flags, 1);
	READ_FLAG(fullscreen, flags, 2);

	return true;
}

bool settings_save(void) {

	FILE *fp = fopen(get_filepath(), "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);

	int flags =
		  (settings_data.coloured_numbers << 0)
		+ (settings_data.secret_five << 1)
		+ (settings_data.fullscreen << 2);

	WRITE_INT(flags);

	return true;
}
