#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include "utility.h"

// how long falling block will travel before collected
// 떨어지는 블록이 받아지기 전까지 얼마나 멀리 떨어지나
const int block_fall_height = 10;

typedef struct {
	int posX; // X 위치 (1, 2, 3)
	int posY; // Y 위치 (0..block_fall_height-1)
} Block;


// create new block
// 새 블록 생성
Block block_new() {
	Block b;
	b.posY = 0;
	b.posX = (utility_random() % 3) + 1;
	return b;
}

// move block downward by 1
// 블록을 한 칸 아래로 떨어뜨리기
void block_make_fall(Block * b) {
	b->posY += 3;
}

// renew block to random position. Intended to reuse single block instance over and over again.
// 블록을 랜덤한 위치에 새로 나타나게 하기. 한 번 선언된 블록을 계속 재사용하기 위함임.
void block_reset(Block * b) {
	b->posX = (utility_random() % 3) + 1;
	b->posY = 0;
}

int block_airborne(Block * b) {
	return b->posY < block_fall_height - 1;
}

#endif
