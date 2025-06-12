#include "exiting.h"
#include "overlay.h"

static int timeout = 0;

bool exiting = false;

void exiting_init(void) {
	hide_overlay();
}

void exiting_update(float dt) {
	timeout++;

	if (timeout > 30)
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
