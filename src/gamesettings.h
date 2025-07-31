#pragma once

#include <stdbool.h>

typedef struct {
	unsigned int flags;
} Settings;

typedef enum {
	FLAG_COLOURED_NUMBERS,
	FLAG_SECRET_FIVE,
	FLAG_FULLSCREEN
} SettingFlags;

Settings *settings(void);

bool settings_getflag(SettingFlags flag);
void settings_toggleflag(SettingFlags flag);

bool settings_load(void);
bool settings_save(void);
