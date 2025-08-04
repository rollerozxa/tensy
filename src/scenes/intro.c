#include "intro.h"
#include "media/textures.h"
#include <stdio.h>

static float intro_timer = 0;

void intro_init(void) {
	intro_timer = 0;
}

void intro_event(const SDL_Event *ev) {
	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_ESCAPE)
		scene_switch("mainmenu");
}

void intro_update(float dt) {
	intro_timer += dt;

	if (intro_timer >= 0.5)
		scene_switch("mainmenu");
}

void intro_draw(SDL_Renderer *renderer) {
	SDL_RenderTexture(renderer, textures_get(TEX_INTRO_RACCOON), NULL, NULL);
	SDL_RenderTexture(renderer, textures_get(TEX_INTRO_TEXT), NULL, NULL);
}

Scene intro_scene = {
	"intro",
	intro_init,
	intro_event,
	intro_update,
	intro_draw,
	0x151515
};
