#pragma once

#include <SDL3/SDL.h>

void AppInit(SDL_Renderer *renderer);
void AppEvent(SDL_Event *ev);
void AppUpdate(void);
void AppDraw(SDL_Renderer *renderer);
void AppQuit(void);
