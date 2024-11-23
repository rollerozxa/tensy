#pragma once

#include <SDL3/SDL.h>

void mainmenu_init();
void mainmenu_event(const SDL_Event *ev);
void mainmenu_update(void);
void mainmenu_draw(SDL_Renderer *renderer);
