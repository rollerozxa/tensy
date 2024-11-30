#include "exiting.h"

static int timeout = 0;

bool exiting = false;

void exiting_update(void) {
	timeout++;

	if (timeout > 30) {
		exiting = true;
	}
}
