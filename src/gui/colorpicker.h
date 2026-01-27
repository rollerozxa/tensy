#pragma once

#include "gui/textinput.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
	SDL_FRect rect;      // area for the colour box
	unsigned int color;  // packed RGB
	TextInput hex;       // hex input box

	// internal state
	double hue;          // 0..1
	double sat;          // 0..1 (selection x)
	double val;          // 0..1 (selection y)
	SDL_FRect _slider_rect; // area for the hue slider
	bool _dragging;      // dragging the colour box
	bool _dragging_hue;  // dragging the hue slider
} ColorPicker;

#define COLORPICKER(obj, rect, initial_hex) \
	obj = (ColorPicker){rect, 0}; \
	colorpicker_init(&obj, initial_hex)

void colorpicker_init(ColorPicker *picker, const char *initial_hex);
bool colorpicker_event(const SDL_Event *ev, ColorPicker *picker);
void colorpicker(ColorPicker *picker);
