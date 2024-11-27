#pragma once

#include <SDL3/SDL.h>

typedef struct {
	bool board_physics;
	bool coloured_numbers;
} Settings;

Settings* settings(void);

void settings_event(const SDL_Event *ev);
void settings_update(void);
void settings_draw(SDL_Renderer *renderer);
