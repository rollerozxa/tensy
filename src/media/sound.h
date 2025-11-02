#pragma once

enum {
	SND_BEAT,
	SND_CLICK,
	SND_FALL,
	SND_MATCH,
	SND_SELECT,
	SND_SHUFFLE,
	SND_WOOZY
};

void sound_init(void);
void sound_play(int sound_id);
