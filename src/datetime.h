#pragma once

#include <SDL3/SDL.h>
#include <stddef.h>

void datetime_formatduration(double seconds_d, char *out, size_t outlen);

SDL_DateTime datetime_now(void);