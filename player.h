#ifndef PLAYER_H
#define PLAYER_H

#define SCORE_UNIT 1

#include <stdio.h>
#include "stage.h"

typedef struct {
	// L/R Position 좌/우 위치 0, 1, 2, 3, 4
	// empty bucket automatically if position is 0 or 4
	// 위치가 0이나 4일 때 자동으로 들고 있는 블록을 없앨 것
	int position;
	// count of blocks holding 들고 있는 블록의 갯수
	int blocks;
	// 점수
	int score;
	// 목숨
	int lives;
} Player;

const int player_HOLD_MAX = 3;

// create new player
// 새 플레이어 생성
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
// 좌로 이동하면서 컨베이어에 있으면 비우기
void player_move_left(Player * p, Stage * s) {
	if (p->position > 0) p->position -= 1;
	
	if (p->position == 0 && s->conveyer_active_position == stage_conveyer_left) {
		p->score += SCORE_UNIT * p->blocks; // increment score 점수 증가
		p->blocks = 0; // empty bucket 쌓인 블록 없애기
	}
}

// move right and empty bucket if the player is in the position of conveyer.
// 우로 이동하면서 컨베이어에 있으면 비우기
void player_move_right(Player * p, Stage * s) {
	if (p->position < 4) p->position += 1;
	
	if (p->position == 4 && s->conveyer_active_position == stage_conveyer_right) {
		p->score += SCORE_UNIT * p->blocks; // increment score 점수 증가
		p->blocks = 0; // empty bucket 쌓인 블록 없애기
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
