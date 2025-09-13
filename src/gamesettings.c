#include "gamesettings.h"

#include <stdio.h>

#include "fileio.h"

static Settings settings_data = {
	  (1ULL << FLAG_SOUND)
	+ (1ULL << FLAG_MUSIC)
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

static const char filever = 1;

static char settings_file[512];

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
	fileio_pref_path(settings_file, sizeof(settings_file), "settings.dat");

	FILE *fp = fopen(settings_file, "rb");
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

	FILE *fp = fopen(settings_file, "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);
	WRITE_INT(settings_data.flags);

	return true;
}
