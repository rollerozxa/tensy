#include "gamesettings.h"
#include "consts.h"
#include "fileio.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static Settings settings_data = {
	0
};

static bool dirty = false;

Settings *settings(void) {
	return &settings_data;
}

void settings_markdirty(void) {
	dirty = true;
}

bool settings_getflag(SettingFlags flag) {
	return settings_data.flags & (1ULL << flag);
}

void settings_toggleflag(SettingFlags flag) {
	dirty = true;
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

static float time_elapsed = 0.f;
void settings_savetimer(float dt) {
	time_elapsed += dt;

	if (dirty && time_elapsed > 60.f) {
		time_elapsed = 0.f;
		dirty = false;
		settings_save();
	}
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
