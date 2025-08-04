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

void toast_show(const char *message, float duration);
void toast_update(float dt);
void toast_draw(SDL_Renderer *renderer);

