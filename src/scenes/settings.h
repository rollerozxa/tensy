#pragma once

#include "scene.h"
#include <SDL3/SDL.h>

typedef struct {
	bool coloured_numbers;
	bool secret_five;
} Settings;

Settings *settings(void);

extern Scene settings_scene;
