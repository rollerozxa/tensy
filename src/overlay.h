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

int add_overlay(Overlay overlay);

int switch_overlay(const char *name);

void hide_overlay(void);
bool has_overlay(void);

void run_overlay_init(void);
void run_overlay_event(const SDL_Event *ev);
void run_overlay_update(void);
void run_overlay_draw(SDL_Renderer *renderer);
