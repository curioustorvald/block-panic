#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "update.h"
#include "utility.h"
#include "block.h"
#include "player.h"
#include "stage.h"

#define SCREEN_W 80
#define SCREEN_H 24

void display_empty_line() {
	int i; for (i = 0; i < SCREEN_W; i++) printf(" ");
}

void display_empty_lines(int lines) {
	int i; for (i = 0; i < lines; i++) display_empty_line();
}

void print_centre(char * text, int y) {
	gotoxy((SCREEN_W - strlen(text)) / 2 + 1, y); printf("%s", text);
}

// Draw game frame
// 게임 화면 틀 그리기
void display_frame() {
	gotoxy(1, 1);
	print_centre("B l o c k   P a n i c !", 1);
	gotoxy(1, 2);
	display_empty_line();
	gotoxy(1, 3);
	printf("=================|");
	gotoxy(24, 3);
	printf("|============|");
	gotoxy(43, 3);
	printf("|============|");
	gotoxy(62, 3);
	printf("|==================");
	gotoxy(1, 4);

	gotoxy(1, 17);
	printf("==O0");
	gotoxy(77, 17);
	printf("0O==");

	gotoxy(1, SCREEN_H);
	printf("  Score");
	gotoxy(68, SCREEN_H);
	printf("Lives");

	gotoxy(1, 13);
	printf("--==");
	gotoxy(77, 13);
	printf("==--");
}

void display_floor() {
	int i;
	for (i = 1; i <= SCREEN_W; i++) {
		gotoxy(i, 19); printf("=");
	}
}

// Draw player
// 플레이어 그리기
int player_pos_x[] = {5, 19, 38, 57, 72};
int player_display_y = 14;
void display_player(Player * p) {
	int i, x;

	for (x = 0; x < sizeof(player_pos_x) / sizeof(int); x++) {
		for (i = 0; i < player_HOLD_MAX + 2; i++) {
			gotoxy(player_pos_x[x], player_display_y + i);
			
			if (x != p->position) {
				printf("     ");
			}
			else {
				if (i == player_HOLD_MAX + 1)
					printf(" O=O ");
				else if (i == player_HOLD_MAX)
					printf("+---+");
				else {
					if (player_HOLD_MAX - i <= p->blocks) {
						if      (p->position == 0) printf(" ===|");
						else if (p->position == 4) printf("|=== ");
						else                       printf("|===|");
					}
					else {
						if      (p->position == 0) printf("    |");
						else if (p->position == 4) printf("|    ");
						else                       printf("|   |");
					}
				}
			}
		}
	}
}

// Draw score and lives
// 점수와 남은 목숨 그리기
void display_player_info(Player * p) {
	gotoxy(10, SCREEN_H);
	printf("%d      ", p->score);
	
	int i;
	
	for (i = 0; i < 3; i++) {
		gotoxy(75 + 2 * i, SCREEN_H);
		
		if (p->lives >= i + 1) {
			printf("@");
		}
		else {
			printf(" ");
		}
	}
}

// 떨어지는 블록 그리기
int block_fall_start = 3;
void display_block(Block * b) {
	int i, x;

	for (i = 0; i < block_fall_height; i++) {
		for (x = 1; x <= 3; x++) {
			gotoxy(player_pos_x[x] + 1, block_fall_start + i);
			if (i == b->posY && x == b->posX)
				printf("===");
			else
				printf("   "); // will wipe out where the block is not on 블록이 없는 곳을 지움
		}
	}
}

// 놓친 블록 그리기 
void display_block_crashed(Block * b) {
	int i, x;
	for (i = 0; i < block_fall_height; i++) {
		for (x = 1; x <= 3; x++) {
			gotoxy(player_pos_x[x] + 1, block_fall_start + i);
			printf("   ");
		}
	}
	
	gotoxy(player_pos_x[b->posX], player_display_y + 4);
	printf("* # *");
}

// 화면 지우기
void display_wipe_screen() {
	#ifdef _WIN32
	system("cls");
	#else
	system("clear");
	#endif

	// for Macs; occasionally the screen won't clean
	gotoxy(1, 1);
	display_empty_lines(SCREEN_H);
	gotoxy(1, 1);
}

// 컨베이어 셔터 그리기 
void display_conveyer_shutter(Stage * s) {
	int i;
	for (i = 0; i < 3; i++) {
		gotoxy(3, 14 + i);
		if (s->conveyer_active_position == stage_conveyer_left)
			printf("  ");
		else
			printf("XX");

		gotoxy(77, 14 + i);
		if (s->conveyer_active_position == stage_conveyer_right)
			printf("  ");
		else
			printf("XX");
	}
}

int __shutter_timer_x;
int __loop_this;
void display_set_shutter_opener(double time_ctr, int time_total, int loop_this, int loop_next) {
	double position_x = round(((double) SCREEN_W) * time_ctr / ((double) time_total));
	
	__loop_this = loop_this;
	
	if (loop_next == 1)
		__shutter_timer_x = SCREEN_W - position_x;
	else if (loop_next == 2)
		__shutter_timer_x = position_x;
}

void display_shutter_open_timer() {	
	int i;
	
	if (__loop_this == 0) {
		for (i = 0; i < SCREEN_W; i++) {			
			gotoxy(i + 1, 19);
			
			if (i == __shutter_timer_x - 1 || i == __shutter_timer_x)
				printf("#");
			else
				printf("=");
		}
	}
}

// draw whole game
// 전체 게임 화면 그리기
void display_all(Player * p, Block * b, Stage * s) {
	display_frame();
	display_conveyer_shutter(s);
	display_player(p);
	display_player_info(p);
	display_block(b);
	display_shutter_open_timer();
}

// 게임 오버 화면 그리기 
void display_game_over(Player * p, Stage * s) {
	char * labels[3];
	labels[0] = "EASY";
	labels[1] = "NORMAL";
	labels[2] = "HARD";
	
	display_wipe_screen();
	display_frame();
	display_floor();
	
	char score_str[32];
	sprintf(score_str, "Score: %d  [%s]", p->score, labels[s->difficulty]);
	print_centre("Game over! Press any key . . . .", player_display_y);
	print_centre(score_str, player_display_y + 2);
}

void display_select_difficulty(int selection) {
	char * label_difficulty = "Difficulty";
	int i;

	char * labels[6];
	labels[0] = "   EASY   ";
	labels[1] = "  NORMAL  ";
	labels[2] = "   HARD   ";
	labels[3] = "[  EASY  ]";
	labels[4] = "[ NORMAL ]";
	labels[5] = "[  HARD  ]";
	
	int labels_count = sizeof(labels) / sizeof(labels[0]) / sizeof(char) / 2;
	
	display_frame();
	
	print_centre(label_difficulty, player_display_y - 6);
	for (i = 0; i < labels_count; i++) {
		print_centre(
				labels[(i == selection) ? i + labels_count : i], 
				player_display_y - 3 + (i * 2)
		);
	}
	
	print_centre("Hit Ctrl+C to quit", SCREEN_H);
}

#endif
