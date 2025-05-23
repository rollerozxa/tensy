#pragma once

// Native resolution at which the game will be rendered
#define NATIVE_WIDTH    640
#define NATIVE_HEIGHT   360

// Size of the screen (scaled up/down from native res)
#define SCREEN_WIDTH    NATIVE_WIDTH*2
#define SCREEN_HEIGHT   NATIVE_HEIGHT*2

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

#define CENTER(outside, inside) ((outside - inside) / 2)
