#pragma once

#include "scene.h"
#include <SDL3/SDL.h>

typedef struct {
	bool board_physics;
	bool coloured_numbers;
} Settings;

Settings *settings(void);

extern Scene settings_scene;
