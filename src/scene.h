#pragma once

#include <SDL3/SDL.h>

typedef struct {
	const char *name;
	void (*init)(void);
	void (*event)(const SDL_Event *);
	void (*update)(void);
	void (*draw)(SDL_Renderer *);
	int colour;
} Scene;

#define MAX_SCENES 10

int add_scene(Scene scene);

int switch_scene(const char *name);

const char *get_current_scene(void);

void run_scene_init(void);
void run_scene_event(const SDL_Event *ev);
void run_scene_update(void);
void run_scene_draw(SDL_Renderer *renderer);

void perform_scene_transition(SDL_Renderer *renderer);
