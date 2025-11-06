#include "highscores.h"
#include "fileio.h"
#include "gamemode.h"
#include <stdio.h>

Highscore highscores[gamemode_count][4][MAX_HIGHSCORES];
static int highscore_count = 0;

void highscore_register(Game state, const char *name) {
	Highscore entry = {
		.score=state.score};

	strncpy(entry.name, name, sizeof(entry.name) - 1);

	int pos = 0;

	Highscore *leaderboard = highscores[state.mode][state.board.boardsize];

	while (pos < MAX_HIGHSCORES && leaderboard[pos].score >= entry.score)
		pos++;

	if (pos >= MAX_HIGHSCORES) // we're off the leaderboard
		return;

	for (int i = MAX_HIGHSCORES - 1; i > pos; i--)
		leaderboard[i] = leaderboard[i - 1];

	leaderboard[pos] = entry;

	highscores_file_save();
}

static const char filever = 3;

static char hs_filepath[512];

// Populate dummy data for testing
bool highscores_populate_dummy(void) {
	for (int i = 0; i < gamemode_count; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < MAX_HIGHSCORES; k++) {
				Game state = {0};
				state.mode = i;
				state.board.boardsize = j;

				state.score = SDL_rand(20000);
				highscore_register(state, "ROllerozxa");
			}
		}
	}
	return false;
}

void highscores_clear(void) {
	for (int i = 0; i < gamemode_count; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < MAX_HIGHSCORES; k++)
				highscores[i][j][k] = (Highscore){0};

	highscores_file_save();
}

bool highscores_file_save(void) {
	FILE *fp = fopen(hs_filepath, "wb");
	if (!fp)
		return false;

	WRITE_CHAR(filever);
	char gmc = gamemode_count;
	WRITE_CHAR(gmc);

	for (int i = 0; i < gamemode_count; i++) {
		WRITE_INT(gamemodes[i].key);

		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < MAX_HIGHSCORES; k++) {
				Highscore *entry = &highscores[i][j][k];
				WRITE_INT(entry->score);
				WRITE_STRING(entry->name, 12);
			}
		}
	}

	fclose(fp);

	game.dirty = false;

	return true;
}

bool highscores_file_load(void) {
	fileio_pref_path(hs_filepath, sizeof(hs_filepath), "highscores.bin");

	//highscores_populate_dummy();
	//highscores_file_save();
	//return true;

	FILE *fp = fopen(hs_filepath, "rb");
	if (!fp)
		return false;

	char ver;
	READ_CHAR(ver);

	if (ver < 3)
		return false; // Version <3 is obsolete

	char gmc;
	READ_CHAR(gmc);

	for (int i = 0; i < gmc; i++) {
		int gm_key;
		READ_INT(gm_key);

		int gamemode_index = gamemode_get_index_by_key(gm_key);

		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < MAX_HIGHSCORES; k++) {
				if (gamemode_index == -1) {
					SDL_Log("What! Unknown gamemode key in highscores file: %d", gm_key);
					fseek(fp, sizeof(int) + 12, SEEK_CUR);
					continue;
				}

				Highscore *entry = &highscores[gamemode_index][j][k];
				READ_INT(entry->score);
				READ_STRING(entry->name, 12);
			}
		}
	}

	fclose(fp);

	return true;
}
