#include "gamesettings.h"
#include "fileio.h"
#include <stdio.h>

static Settings settings_data = {
	  (1ULL << FLAG_SOUND)
	+ (1ULL << FLAG_MUSIC),
	0.0f,
	0, 0,
	""
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
	settings_data.playtime += dt;

	// Autosave every 15 minutes if nothing changed, or every minute if something important did.
	if ((dirty && time_elapsed > 60.f) || time_elapsed > 15.f*60.f) {
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
	READ_DOUBLE(settings_data.playtime);
	READ_LONG(settings_data.numbers_removed);
	READ_LONG(settings_data.total_accumulated_score);
	READ_STRING(settings_data.last_username, 12);

	fclose(fp);

	return true;
}

bool settings_save(void) {

	FILE *fp = fopen(settings_file, "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);
	WRITE_INT(settings_data.flags);
	WRITE_DOUBLE(settings_data.playtime);
	WRITE_LONG(settings_data.numbers_removed);
	WRITE_LONG(settings_data.total_accumulated_score);
	WRITE_STRING(settings_data.last_username, 12);

	fclose(fp);

	return true;
}
