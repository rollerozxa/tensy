#pragma once

#include <SDL3/SDL.h>

bool virtual_cursor_is_active(void);

SDL_FPoint virtual_cursor_get_pos(void);
int virtual_cursor_clicked(void);

void virtual_cursor_event(const SDL_Event *ev);
void virtual_cursor_update(void);
void virtual_cursor_draw(void);

