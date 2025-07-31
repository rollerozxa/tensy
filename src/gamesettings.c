#include "gamesettings.h"
#include "consts.h"
#include "fileio.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static Settings settings_data = {
	0
};

Settings *settings(void) {
	return &settings_data;
}

bool settings_getflag(SettingFlags flag) {
	return settings_data.flags & (1ULL << flag);
}

void settings_toggleflag(SettingFlags flag) {
	settings_data.flags = settings_data.flags ^ (1ULL << flag);
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

bool settings_load(void) {
	FILE *fp = fopen(get_filepath(), "rb");
	if (!fp)
		return false;

	char tmp;
	READ_CHAR(tmp);
	if (tmp != filever)
		return false; // uhh

	READ_INT(settings_data.flags);

	return true;
}

bool settings_save(void) {

	FILE *fp = fopen(get_filepath(), "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);
	WRITE_INT(settings_data.flags);

	return true;
}
