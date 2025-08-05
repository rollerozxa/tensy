#pragma once

#include <SDL3/SDL.h>

typedef struct {
	const char *name;
	void (*init)(void);
	void (*event)(const SDL_Event *);
	void (*update)(float dt);
	void (*draw)(void);
	int color;
} Scene;

#define MAX_SCENES 10

int scene_add(Scene scene);

int scene_switch(const char *name);

const char *scene_get_current(void);

void scene_run_init(void);
void scene_run_event(const SDL_Event *ev);
void scene_run_update(float dt);
void scene_run_draw(void);

void scene_perform_transition(void);
bool scene_is_transitioning(void);
