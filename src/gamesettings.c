#include "gamesettings.h"

static Settings settings_data = {
	false,
	false,
};

Settings *settings(void) {
	return &settings_data;
}
