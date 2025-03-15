#include "scenes.h"
#include "exiting.h"
#include "game.h"
#include "mainmenu.h"
#include "overlay.h"
#include "pause.h"
#include "scene.h"
#include "settings.h"

// First scene needs to be first
Scene scenes[] = {
	{"mainmenu", mainmenu_init, mainmenu_event, mainmenu_update, mainmenu_draw},
	{"game", game_init, game_event, game_update, game_draw},
	{"settings", NULL, settings_event, settings_update, settings_draw},
	{"exiting", NULL, NULL, exiting_update, NULL}
};
static size_t scene_count = sizeof(scenes) / sizeof(scenes[0]);

Overlay overlays[] = {
	{"pause", NULL, pause_event, NULL, pause_draw}
};
static size_t overlay_count = sizeof(overlays) / sizeof(overlays[0]);

void register_scenes(void) {
	for (size_t i = 0; i < scene_count; i++) {
		add_scene(scenes[i]);
	}

	for (size_t i = 0; i < overlay_count; i++) {
		add_overlay(overlays[i]);
	}
}
