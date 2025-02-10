#pragma once

#include <SDL3/SDL.h>

void pause_init(void);
void pause_event(const SDL_Event *ev);
void pause_update(void);
void pause_draw(SDL_Renderer *renderer);
