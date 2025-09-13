#include "scenes.h"

#include "overlay.h"
#include "scene.h"

extern Scene about_scene;
extern Scene credits_scene;
extern Scene devintro_scene;
extern Scene exiting_scene;
extern Scene game_scene;
extern Scene gameconfig_scene;
extern Scene intro_scene;
extern Scene leaderboard_scene;
extern Scene mainmenu_scene;
extern Scene selectmode_scene;
extern Scene settings_scene;
extern Scene statistics_scene;

extern Overlay endgame_overlay;
extern Overlay exitconfirm_overlay;
extern Overlay pause_overlay;
extern Overlay shuffle_overlay;
extern Overlay timeout_overlay;

void scenes_register(void) {
	// First scene needs to be first
#ifndef DEBUG
	scene_add(devintro_scene);
#endif
	scene_add(intro_scene);
	scene_add(credits_scene);
	scene_add(mainmenu_scene);
	scene_add(about_scene);
	scene_add(exiting_scene);
	scene_add(game_scene);
	scene_add(gameconfig_scene);
	scene_add(leaderboard_scene);
	scene_add(selectmode_scene);
	scene_add(settings_scene);
	scene_add(statistics_scene);

	overlay_add(endgame_overlay);
	overlay_add(exitconfirm_overlay);
	overlay_add(pause_overlay);
	overlay_add(shuffle_overlay);
	overlay_add(timeout_overlay);
}
