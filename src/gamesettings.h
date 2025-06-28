#pragma once

#include <stdbool.h>

typedef struct {
	bool coloured_numbers;
	bool secret_five;
} Settings;

Settings *settings(void);

