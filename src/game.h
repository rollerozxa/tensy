#pragma once

#include <SDL3/SDL.h>

void game_init(void);
void game_event(const SDL_Event *ev);
void game_update(void);
void game_draw(SDL_Renderer *renderer);
