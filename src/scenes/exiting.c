#include "exiting.h"

static int timeout = 0;

bool exiting = false;

void exiting_update(void) {
	timeout++;

	if (timeout > 30)
		exiting = true;
}

Scene exiting_scene = {
	"exiting",
	NULL,
	NULL,
	exiting_update,
	NULL,
	0
};
