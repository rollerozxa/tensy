#include "timeformat.h"
#include <stdio.h>
#include <string.h>

/**
 * Format a duration of seconds into a human-readable string.
 * (days, hours, minutes and seconds - the two largest non-zero units are shown)
 */
void timeformat_duration(double seconds_d, char *out, size_t outlen) {
	unsigned long seconds = (unsigned long)seconds_d;

	struct { const char *suf; unsigned int sec; } units[] = {
		{ "d", 86400 },
		{ "h", 3600 },
		{ "m", 60 },
		{ "s", 1 }
	};

	out[0] = '\0';
	int parts = 0;

	for (size_t i = 0; i < sizeof(units) / sizeof(units[0]); i++) {
		if (seconds < units[i].sec)
			continue;

		if (parts > 0)
			strcat(out, ", ");

		unsigned long value = seconds / units[i].sec;
		seconds = seconds % units[i].sec;
		size_t used = strlen(out);

		snprintf(out + used, outlen - used, "%lu%s", value, units[i].suf);

		parts++;
		if (parts == 2)
			break;
	}
}
