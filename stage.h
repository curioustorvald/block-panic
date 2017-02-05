#ifndef STAGE_H
#define STAGE_H

// Ticks: MUST BE COPRIME OF THE OPERATE_TICK
// Tick값: OPERATE_TICK과 서로소가 되는 수로 설정할 것 
#define TICK_EASY   659
#define TICK_NORMAL 419
#define TICK_HARD   277

#include <stdio.h>

const int stage_conveyer_left = 1;
const int stage_conveyer_right = 2;

int difficulty[] = {TICK_EASY, TICK_NORMAL, TICK_HARD};

typedef struct {
	// in milliseconds. lower == faster == harder
	// must get smaller as the game progresses. // ADDENDUM: apparently it was not implemented. -- Torvald on 2017-02-06
	// 밀리초 단위. 낮을수록 빨라지고 어려워짐.
	int game_tick;
	// which conveyer is now open
	// 0: closed, 1: left, 2: right
	// 어느 쪽 컨베이어가 열려 있나
	// 0: 닫힘, 1: 좌, 2: 우
	int conveyer_active_position;
	
	int difficulty; //0: Easy, 1: Normal, 2: Hard
} Stage;

// 새 스테이지
// d: difficulty code 난이도 코드
//    0: EASY, 1: NORMAL, 2: HARD 
Stage stage_new(int d) {
	Stage s;
	s.game_tick = difficulty[d];
	s.difficulty = d;
	s.conveyer_active_position = 0;
	return s;
}

#endif
