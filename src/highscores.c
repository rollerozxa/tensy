#include "highscores.h"
#include "fileio.h"
#include <stdio.h>

Highscore hs[MAX_HIGHSCORES];
static int highscore_count = 0;

Highscore *highscores(void) {
	return hs;
}

void highscore_register(Game state) {
	Highscore entry = {
		.score=state.score,
		.gamemode=state.mode,
		.size=state.board.boardsize,
		.flags=0};

	int pos = 0;

	while (pos < MAX_HIGHSCORES && hs[pos].score >= entry.score)
		pos++;

	if (pos >= MAX_HIGHSCORES) // we're off the leaderboard
		return;

	for (int i = MAX_HIGHSCORES - 1; i > pos; i--)
		hs[i] = hs[i - 1];

	hs[pos] = entry;

	highscores_file_save();
}

static const char filever = 1;

static char hs_filepath[512];

bool highscores_file_save(void) {
	fileio_pref_path(hs_filepath, sizeof(hs_filepath), "highscores.bin");

	FILE *fp = fopen(hs_filepath, "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);

	for (int i = 0; i < MAX_HIGHSCORES; i++) {
		WRITE_LONG(hs[i].score);
		WRITE_CHAR(hs[i].gamemode);
		WRITE_CHAR(hs[i].size);
		WRITE_CHAR(hs[i].flags);
	}

	fclose(fp);

	game.dirty = false;

	return true;
}

bool highscores_file_load(void) {
	FILE *fp = fopen(hs_filepath, "rb");
	if (!fp)
		return false;

	char tmp;
	READ_CHAR(tmp);
	if (tmp != filever)
		return false; // uhh

	for (int i = 0; i < MAX_HIGHSCORES; i++) {
		READ_LONG(hs[i].score);
		READ_CHAR(hs[i].gamemode);
		READ_CHAR(hs[i].size);
		READ_CHAR(hs[i].flags);
	}

	fclose(fp);

	return true;
}
