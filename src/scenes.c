#include "scenes.h"
#include "exiting.h"
#include "game.h"
#include "mainmenu.h"
#include "overlay.h"
#include "pause.h"
#include "scene.h"
#include "settings.h"

void register_scenes(void) {
	// First scene needs to be first
	add_scene((Scene){"mainmenu", mainmenu_init, mainmenu_event, mainmenu_update, mainmenu_draw});

	add_scene((Scene){"game", game_init, game_event, game_update, game_draw});

	add_scene((Scene){"settings", NULL, settings_event, settings_update, settings_draw});

	add_scene((Scene){"exiting", NULL, NULL, exiting_update, NULL});

	add_overlay((Overlay){"pause", NULL, pause_event, NULL, pause_draw});
}
