#include "consts.h"
#include "draw.h"
#include "media/music.h"
#include "media/textures.h"
#include "scene.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static float intro_timer = 0;

void intro_init(void) {
	intro_timer = 0;
	music_play(MUS_MAINMENU, -1);
}

void intro_event(const SDL_Event *ev) {
	if (ev->type == SDL_EVENT_KEY_DOWN && ev->key.scancode == SDL_SCANCODE_ESCAPE)
		scene_switch("mainmenu");
}

void intro_update(float dt) {
	if (scene_is_transitioning())
		return;

	intro_timer += dt;

	if (intro_timer >= 0.5 && !scene_is_transitioning())
		scene_switch("mainmenu");
}

void intro_draw(void) {
	draw_texture(TEX_INTRO_RACCOON, NULL, &FULL_RECT());
	draw_texture(TEX_INTRO_TEXT, NULL, &FULL_RECT());
}

Scene intro_scene = {
	"intro",
	intro_init,
	intro_event,
	intro_update,
	intro_draw,
	0x151515
};
