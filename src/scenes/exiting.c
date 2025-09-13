#include "exiting.h"

#include <stdbool.h>
#include <stddef.h>

#include "gamesettings.h"
#include "overlay.h"

static float timeout = 0;

bool exiting = false;

void exiting_init(void) {
	overlay_hide();
}

void exiting_update(float dt) {
	timeout += dt;

	if (timeout > 0.35 || settings_getflag(FLAG_REDUCED_MOTION))
		exiting = true;
}

Scene exiting_scene = {
	"exiting",
	exiting_init,
	NULL,
	exiting_update,
	NULL,
	0
};
