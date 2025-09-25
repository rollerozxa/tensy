#include "gamestate.h"
#include "gamemode.h"
#include "overlay.h"
#include "savestate.h"
#include <stdio.h>
#include <stdlib.h>

Game game = {
	.board = {NULL, 30, 15, 2},
	.score = 0,
	.time_left = 0.f,
	.total_time = 0.f,
	.mode = GM_Leisure,
	.loaded_existing = false,
	.shuffles = 0,
	.dirty = true,
	.dead = false
};

typedef struct ChkNode {
	struct ChkNode *prev;
	Checkpoint checkpoint;
} ChkNode;

static ChkNode *head = NULL;

void gamestate_checkpoint(void) {
	game.dirty = true;

	ChkNode *node = malloc(sizeof(ChkNode));

	board_copy(&node->checkpoint.board, &game.board);
	node->checkpoint.score = game.score;
	node->checkpoint.shuffles = game.shuffles;

	node->prev = head;
	head = node;
}

bool gamestate_has_undo(void) {
	return head != NULL;
}

void gamestate_undo(void) {
	if (!head)
		return;

	game.dirty = true;

	ChkNode *node = head;
	head = node->prev;

	board_copy(&game.board, &node->checkpoint.board);
	game.score = node->checkpoint.score;
	game.shuffles = node->checkpoint.shuffles;

	board_free(&node->checkpoint.board);
	free(node);
}

void gamestate_clear(void) {
	game.dirty = true;

	while (head) {
		ChkNode *node = head;
		head = node->prev;

		if (node->checkpoint.board.p)
			board_free(&node->checkpoint.board);

		free(node);
	}
}

void gamestate_traverse(void) {
	ChkNode *node = head;

	while (node != NULL) {
		printf("%d <- ", node->checkpoint.score);
		node = node->prev;
	}

	printf("NULL\n");
}

void gamestate_gameover(void) {
	overlay_switch("gameover");
	uint64_t identifier = savestate_read_identifier();
	if (identifier == game.identifier) {
		// destroy savestate
		savestate_delete();
	}
	game.dead = true;
}
