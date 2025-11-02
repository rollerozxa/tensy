#pragma once

#include <SDL3/SDL.h>
#include <stdio.h>

// Native resolution at which the game will be rendered
#define SCREEN_W 640
#define SCREEN_H 360

// Size of the window (scaled up/down from native res)
#define WINDOW_W SCREEN_W*2
#define WINDOW_H SCREEN_H*2

// Shared constant for app name
#define APP_NAME "Tensy"
// Shared constant for app ID
#define APP_ID "se.voxelmanip.tensy"
// Shared constant for app "org"
#define APP_ORG "ROllerozxa"

// Misc helper macros

#define RECTCPY(rect) {rect.x, rect.y, rect.w, rect.h}

#define RECT(x,y,w,h) ((SDL_FRect){x,y,w,h})
#define POINT(x,y) ((SDL_FPoint){x,y})

#define FULL_RECT() RECT(0, 0, SCREEN_W, SCREEN_H)

#define CENTER(outside, inside) ((outside - inside) / 2)

#define FMT_STRING(buf, size, fmt, ...) \
	char (buf)[(size)]; \
	snprintf((buf), (size), (fmt), ##__VA_ARGS__)

#if defined(SDL_PLATFORM_ANDROID) || defined(SDL_PLATFORM_VITA)
	#define ALWAYS_FULLSCREEN true
#endif

#define STR(x) SDL_STRINGIFY_ARG(x)
