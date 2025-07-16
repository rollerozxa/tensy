#pragma once

#include <stdbool.h>

typedef struct {
	bool coloured_numbers;
	bool secret_five;
	bool fullscreen;
} Settings;

Settings *settings(void);

bool settings_load(void);
bool settings_save(void);
