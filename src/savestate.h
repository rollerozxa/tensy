#pragma once

#include <stdbool.h>
#include <stdint.h>

bool savestate_exists(void);
bool savestate_delete(void);
bool savestate_save(void);
bool savestate_load(void);
uint64_t savestate_read_identifier(void);
