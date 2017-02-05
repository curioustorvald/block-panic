#ifndef BLOCK_H
#define BLOCK_H

#include <stdio.h>
#include "utility.h"

// how long falling block will travel before collected
// �������� ����� �޾����� ������ �󸶳� �ָ� ��������
const int block_fall_height = 10;

typedef struct {
	int posX; // X ��ġ (1, 2, 3)
	int posY; // Y ��ġ (0..block_fall_height-1)
} Block;


// create new block
// �� ��� ����
Block block_new() {
	Block b;
	b.posY = 0;
	b.posX = (utility_random() % 3) + 1;
	return b;
}

// move block downward by 1
// ����� �� ĭ �Ʒ��� ����߸���
void block_make_fall(Block * b) {
	b->posY += 3;
}

// renew block to random position. Intended to reuse single block instance over and over again.
// ����� ������ ��ġ�� ���� ��Ÿ���� �ϱ�. �� �� ����� ����� ��� �����ϱ� ������.
void block_reset(Block * b) {
	b->posX = (utility_random() % 3) + 1;
	b->posY = 0;
}

int block_airborne(Block * b) {
	return b->posY < block_fall_height - 1;
}

#endif
