#pragma once

#include <SDL3/SDL.h>

typedef enum VirtualCursorState {
	VC_INACTIVE,
	VC_ACTIVE,
	VC_FORCE_OFF
} VirtualCursorState;

VirtualCursorState virtual_cursor_get_state(void);
void virtual_cursor_set_state(VirtualCursorState state);

SDL_FPoint virtual_cursor_get_pos(void);
int virtual_cursor_clicked(void);

void virtual_cursor_event(const SDL_Event *ev);
void virtual_cursor_update(void);
void virtual_cursor_draw(void);

