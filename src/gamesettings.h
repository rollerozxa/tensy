#pragma once

#include <stdbool.h>

typedef struct {
	unsigned int flags;
} Settings;

typedef enum {
	FLAG_MONO_NUMBERS,
	FLAG_SECRET_FIVE,
	FLAG_FULLSCREEN,
	FLAG_SOUND,
	FLAG_REDUCED_MOTION
} SettingFlags;

Settings *settings(void);

bool settings_getflag(SettingFlags flag);
void settings_toggleflag(SettingFlags flag);

void settings_savetimer(float dt);

bool settings_load(void);
bool settings_save(void);
