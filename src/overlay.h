#pragma once

#include <SDL3/SDL.h>

typedef struct {
	const char *name;
	void (*init)(void);
	void (*event)(const SDL_Event *);
	void (*update)(void);
	void (*draw)(SDL_Renderer *);
} Overlay;

#define MAX_OVERLAYS 10

int overlay_add(Overlay overlay);

int overlay_switch(const char *name);

const char *overlay_get_current(void);

void overlay_hide(void);
bool overlay_exists(void);

void overlay_run_init(void);
void overlay_run_event(const SDL_Event *ev);
void overlay_run_update(void);
void overlay_run_draw(SDL_Renderer *renderer);
