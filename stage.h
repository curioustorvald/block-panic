#ifndef STAGE_H
#define STAGE_H

// Ticks: MUST BE COPRIME OF THE OPERATE_TICK
// Tick��: OPERATE_TICK�� ���μҰ� �Ǵ� ���� ������ �� 
#define TICK_EASY   659
#define TICK_NORMAL 419
#define TICK_HARD   277

#include <stdio.h>

const int stage_conveyer_left = 1;
const int stage_conveyer_right = 2;

int difficulty[] = {TICK_EASY, TICK_NORMAL, TICK_HARD};

typedef struct {
	// in milliseconds. lower == faster == harder
	// must get smaller as the game progresses.
	// �и��� ����. �������� �������� �������.
	int game_tick;
	// which conveyer is now open
	// 0: closed, 1: left, 2: right
	// ��� �� �����̾ ���� �ֳ�
	// 0: ����, 1: ��, 2: ��
	int conveyer_active_position;
	
	int difficulty; //0: Easy, 1: Normal, 2: Hard
} Stage;

// �� ��������
// d: difficulty code ���̵� �ڵ�
//    0: EASY, 1: NORMAL, 2: HARD 
Stage stage_new(int d) {
	Stage s;
	s.game_tick = difficulty[d];
	s.difficulty = d;
	s.conveyer_active_position = 0;
	return s;
}

#endif
