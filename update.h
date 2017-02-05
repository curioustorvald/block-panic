#ifndef UPDATE_H
#define UPDATE_H

#include <stdio.h>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

double update_time_delta = 0.0; // in milliseconds

#include "display.h"
#include "player.h"
#include "block.h"
#include "stage.h"
#include "utility.h"

#ifdef _WIN32
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_ENTER 13
#else
#define KEY_LEFT  68
#define KEY_RIGHT 67
#define KEY_UP    65
#define KEY_DOWN  66
#define KEY_ENTER 10
#endif

#define OOPS_TIME 2000

// if the execution time exceeds this threshold, time delta won't be updated.
// 실행 시간이 이 값 이상이면 time delta를 업데이트하지 않음 
#define TIME_DELTA_DISCARD_THRE 70

clock_t begin, end;

double __time_accum_fall = 0.0; // time accumulator for block fall
double __time_accum_shutter = 0.0; // time accumulator for shutter control

int __b_debug = 0;

void update_delta() {
	// save the execution time as time delta
	// 실행 시간의 차를 delta로 저장 
	end = clock();

	update_time_delta = (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC;

	if (__b_debug) {
		gotoxy(1, 1);
		printf("TD %.3f ms  ", update_time_delta);
	}

	// if time delta is below the threshold then...
	// 실행 시간의 차가 설정된 값보다 작으면...
	if (update_time_delta < TIME_DELTA_DISCARD_THRE) {
		__time_accum_fall += update_time_delta;
		__time_accum_shutter += update_time_delta;
	}

	begin = clock();
}

void update_control(Player * p, Stage * s) {
	int key = get_key();
	
	if (key == KEY_LEFT) {
		player_move_left(p, s);
	}
	else if (key == KEY_RIGHT) {
		player_move_right(p, s);
	}
}

int shutter_loop[] = {1, 1, 0, 2, 2, 0}; // 셔터 상태 패턴 
int __shutter_loop_index = 5; // 현재 패턴 
int __shutter_active_time = 1000; // 각 패턴의 지속 시간 
int __shutter_loop_pattern_size = sizeof(shutter_loop) / sizeof(int);
void update_shutter(Stage * s) {
	if (__time_accum_shutter >= __shutter_active_time) {
		__shutter_loop_index = (__shutter_loop_index + 1) % __shutter_loop_pattern_size;
		
		__time_accum_shutter -= __shutter_active_time;
	}
	
	s->conveyer_active_position = shutter_loop[__shutter_loop_index];
	
	display_set_shutter_opener(__time_accum_shutter, 
			__shutter_active_time,
			shutter_loop[__shutter_loop_index],
			shutter_loop[(__shutter_loop_index + 1) % __shutter_loop_pattern_size]
	);
}

void update_reset_game(Player * p) {
	__shutter_loop_index = 5;
	__time_accum_shutter = 0;
	player_reset(p);
}

void update_block(Player * p, Block * b, Stage * s) {
	// if accmulated time >= game tick (i.e. if it's about time then)
	if (__time_accum_fall >= s->game_tick) {
		// make block fall if it's airborne
		if (block_airborne(b))
			block_make_fall(b);
		// if not, catch it or punish player
		else {
			// if player was aligned with block
			if (p->position == b->posX && player_has_margin(p)) {
				// player's bucket has some space; SUCCESS
				player_add_block(p);
			}
			// player was not aligned or has no empty space; punish it!
			else {
				player_kill(p);
				display_block_crashed(b);

				utility_sleep(OOPS_TIME);
				
				update_reset_game(p);
			}
			
			// deploy new block
			block_reset(b);
		}
		
		__time_accum_fall -= s->game_tick;
	}
}


// update whole game
// 전체 게임 업데이트 
void update_all(Player * p, Block * b, Stage * s) {
	update_delta();

	update_control(p, s);
	update_block(p, b, s);
	update_shutter(s);
}

int difficulty_selection = 1;
int difficulty_count = 3;
int update_select_difficulty() {
	int key = get_key();
	
	if (key == KEY_UP && difficulty_selection > 0)
		difficulty_selection--;
	else if (key == KEY_DOWN && difficulty_selection < difficulty_count - 1)
		difficulty_selection++;
	else if (key == KEY_ENTER) {
		return 1;
	}
		
	return 0;
}

#endif
