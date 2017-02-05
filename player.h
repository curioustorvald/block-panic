#ifndef PLAYER_H
#define PLAYER_H

#define SCORE_UNIT 1

#include <stdio.h>
#include "stage.h"

typedef struct {
	// L/R Position ��/�� ��ġ 0, 1, 2, 3, 4
	// empty bucket automatically if position is 0 or 4
	// ��ġ�� 0�̳� 4�� �� �ڵ����� ��� �ִ� ����� ���� ��
	int position;
	// count of blocks holding ��� �ִ� ����� ����
	int blocks;
	// ����
	int score;
	// ���
	int lives;
} Player;

const int player_HOLD_MAX = 3;

// create new player
// �� �÷��̾� ����
Player player_new() {
	Player p;
	p.position = 2;
	p.blocks = 0;
	p.score = 0;
	p.lives = 3;

	return p;
}

void player_reset(Player * p) {
	p->blocks = 0;
}

// move left and empty bucket if the player is in the position of conveyer.
// �·� �̵��ϸ鼭 �����̾ ������ ����
void player_move_left(Player * p, Stage * s) {
	if (p->position > 0) p->position -= 1;
	
	if (p->position == 0 && s->conveyer_active_position == stage_conveyer_left) {
		p->score += SCORE_UNIT * p->blocks; // increment score ���� ����
		p->blocks = 0; // empty bucket ���� ��� ���ֱ�
	}
}

// move right and empty bucket if the player is in the position of conveyer.
// ��� �̵��ϸ鼭 �����̾ ������ ����
void player_move_right(Player * p, Stage * s) {
	if (p->position < 4) p->position += 1;
	
	if (p->position == 4 && s->conveyer_active_position == stage_conveyer_right) {
		p->score += SCORE_UNIT * p->blocks; // increment score ���� ����
		p->blocks = 0; // empty bucket ���� ��� ���ֱ�
	}
}

void player_add_block(Player * p) {
	p->blocks += 1;
	p->score += SCORE_UNIT;
}

void player_kill(Player * p) {
	p->lives -= 1;
}

int player_has_margin(Player * p) {
	return p->blocks < player_HOLD_MAX;
}

#endif
