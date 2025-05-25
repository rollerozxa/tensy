#pragma once

#include <SDL3/SDL.h>

typedef struct {
    const char *message;
    float scale;
    float duration;
    float timer;
    float alpha;
    bool active;
} Toast;

void show_toast(const char *message, float duration);
void update_toast(float dt);
void draw_toast(SDL_Renderer *renderer);

