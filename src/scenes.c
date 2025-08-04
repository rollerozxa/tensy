#include "scenes.h"
#include "overlays/exitconfirm.h"
#include "overlays/pause.h"
#include "overlays/shuffle.h"
#include "overlays/timeout.h"
#include "scenes/about.h"
#include "scenes/exiting.h"
#include "scenes/game.h"
#include "scenes/gameconfig.h"
#include "scenes/intro.h"
#include "scenes/mainmenu.h"
#include "scenes/selectmode.h"
#include "scenes/settings.h"

void scenes_register(void) {
	// First scene needs to be first
	scene_add(intro_scene);
	scene_add(about_scene);
	scene_add(mainmenu_scene);
	scene_add(exiting_scene);
	scene_add(game_scene);
	scene_add(gameconfig_scene);
	scene_add(selectmode_scene);
	scene_add(settings_scene);

	overlay_add(exitconfirm_overlay);
	overlay_add(pause_overlay);
	overlay_add(shuffle_overlay);
	overlay_add(timeout_overlay);
}
