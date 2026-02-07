#include "scenes.h"
#include "overlay.h"
#include "scene.h"
#include "version_info.h"

#if defined(DEBUG) || !VER_DEV
	#define START_SCENE "intro"
#else
	#define START_SCENE "devintro"
#endif

extern Scene about_scene;
extern Scene clear_data_scene;
extern Scene credits_scene;
extern Scene customize_colors_scene;
extern Scene devintro_scene;
extern Scene exiting_scene;
extern Scene game_scene;
extern Scene gameconfig_scene;
extern Scene intro_scene;
extern Scene leaderboard_scene;
extern Scene mainmenu_scene;
extern Scene selectmode_scene;
extern Scene settings_scene;
extern Scene statistics_numbers_scene;
extern Scene statistics_scene;

extern Overlay endgame_overlay;
extern Overlay exitconfirm_overlay;
extern Overlay game_statistics_numbers_overlay;
extern Overlay gameover_overlay;
extern Overlay pause_overlay;
extern Overlay shuffle_overlay;

void scenes_register(void) {
	scene_add(about_scene);
	scene_add(clear_data_scene);
	scene_add(credits_scene);
	scene_add(customize_colors_scene);
	scene_add(devintro_scene);
	scene_add(exiting_scene);
	scene_add(game_scene);
	scene_add(gameconfig_scene);
	scene_add(intro_scene);
	scene_add(leaderboard_scene);
	scene_add(mainmenu_scene);
	scene_add(selectmode_scene);
	scene_add(settings_scene);
	scene_add(statistics_numbers_scene);
	scene_add(statistics_scene);

	scene_switch_instant(START_SCENE);

	overlay_add(endgame_overlay);
	overlay_add(exitconfirm_overlay);
	overlay_add(game_statistics_numbers_overlay);
	overlay_add(gameover_overlay);
	overlay_add(pause_overlay);
	overlay_add(shuffle_overlay);
}
