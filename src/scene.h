#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
	const char *name;
	void (*init)(void);
	void (*event)(const SDL_Event *);
	void (*update)(float dt);
	void (*draw)(void);
	int color;
} Scene;

#define MAX_SCENES 256

int scene_add(Scene scene);

void scene_switch(const char *name);
void scene_switch_instant(const char *name);

const char *scene_get_current(void);

void scene_run_init(void);
void scene_run_event(const SDL_Event *ev);
void scene_run_update(float dt);
void scene_run_draw(void);

void scene_perform_transition(void);
bool scene_is_transitioning(void);
