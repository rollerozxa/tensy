#pragma once

#include <stdbool.h>

bool savestate_exists(void);
bool savestate_save(void);
bool savestate_load(void);
