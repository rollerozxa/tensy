#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	unsigned int flags;
	double playtime;
	uint64_t numbers_removed;
	uint64_t total_accumulated_score;
	char last_username[12];
	uint32_t numbers[9];
} Settings;

typedef enum {
	FLAG_MONO_NUMBERS,
	FLAG_SECRET_FIVE,
	FLAG_FULLSCREEN,
	FLAG_SOUND,
	FLAG_REDUCED_MOTION,
	FLAG_MUSIC,
	FLAG_PIXEL_PERFECT,
} SettingFlags;

Settings *settings(void);

bool settings_getflag(SettingFlags flag);
void settings_toggleflag(SettingFlags flag);

void settings_savetimer(float dt);

bool settings_load(void);
bool settings_save(void);

void settings_clear(void);
