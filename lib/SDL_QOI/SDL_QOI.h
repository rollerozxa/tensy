#pragma once

#include <SDL3/SDL.h>

SDL_Surface * SDLCALL SDL_LoadQOI_IO(SDL_IOStream *src);

#define SDL_LoadQOI(file) SDL_LoadQOI_IO(SDL_IOFromFile(file, "rb"))
