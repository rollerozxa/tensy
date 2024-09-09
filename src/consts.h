#pragma once

// Native resolution at which the game will be rendered
#define NATIVE_WIDTH    640
#define NATIVE_HEIGHT   360

// Size of the screen (scaled up/down from native res)
#define SCREEN_WIDTH    NATIVE_WIDTH*2
#define SCREEN_HEIGHT   NATIVE_HEIGHT*2

#define GLYPH_WIDTH 6
#define GLYPH_HEIGHT 12


// Max scenes to be allocated
#define MAX_SCENES 10


// Misc helper macros

#define RECT(x,y,w,h) &((SDL_FRect){x,y,w,h})
