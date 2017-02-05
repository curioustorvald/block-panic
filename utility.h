#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

#include "update.h"

// getch and kbhit implementation for non-Windows system
// 비 윈도 시스템용 getch와 kbhit 
#ifndef _WIN32
static struct termios oldch, newch;
static struct termios oldt, newt;
// snippet from "www.thecodingforums.com/threads/replacement-for-_getch.966464/"
char _getch(void) {
	tcgetattr(0, &oldch); // grab old terminal i/o settings

	newch = oldt; // make new settings same as old settings 
	newch.c_lflag &= ~ICANON; // disable buffered i/o
	newch.c_lflag &= ~ECHO; // set echo mode
	
	tcsetattr(0, TCSANOW, &newch); // use these new terminal i/o settings now
	
	char ch;
	ch = getchar();

	//tcsetattr(0, TCSANOW, &oldch); // Restore old terminal i/o settings
	
	return ch;
}

// snippet from "cboard.cprogramming.com/c-programming/63166-kbhit-linux.html"
int kbhit(void) {
	int ch;
	int oldf;
	
	tcgetattr(STDIN_FILENO, &oldt); // grab old terminal i/o settings
	
	newt = oldt; // make new settings same as old settings
	newt.c_lflag &= ~ICANON; // disable buffered i/o
	newt.c_lflag &= ~ECHO; // set echo mode
	
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); // use these new terminal i/o settings now
	
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	
	ch = getchar();
	
	//tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old terminal i/o settings
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	if(ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}
	
	return 0;
}
#endif

// Move cursor to (x, y)
// 커서를 (x, y)로 이동
void gotoxy(int x, int y) {
	#ifdef _WIN32 // 윈도일 때
	COORD coord;
	coord.X = x - 1;
	coord.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	#else // 윈도가 아닐 때
	printf("%c[%d;%df", 0x1B, y, x);
	#endif
}

// sleep process in millisec
// millisec(밀리초)만큼 프로세스 잠재우기
double __utility_sleep_time_accum = 0.0;
int a = 1;
void utility_sleep(int millisec) {
	#ifdef _WIN32 // 윈도일 때
	Sleep(millisec); // defined in windows.h
	#else // 윈도가 아닐 때
	fflush(stdout); // if print function precedes this sleep, make sure the print work by flushing the buffer
	usleep(millisec * 1000); // actual sleep in nanoseconds
	#endif
}

// snippet from "stackoverflow.com/questions/10463201/getch-and-arrow-codes"
int get_arrow_key_unix() {
	char ch = _getch();
	if (ch == '\033') { // if the first value is esc
    	_getch(); // skip the [
    	switch(_getch()) { // the real value
    	    case 'A':
    	        return 'A';
    	    case 'B':
    	        return 'B';
    	    case 'C':
    	        return 'C';
    	    case 'D':
    	        return 'D';
    	    default:
    	    	return ch;
    	}
    }
    else {
    	return ch;
    }
}

// Get key press and return it
// 키 입력을 받아 키 값을 반환
int get_key() {
	int key = 0;
	if (kbhit()) {
		#ifdef _WIN32
		key = _getch();
		#else
		key = get_arrow_key_unix();
		#endif
	}

	return key;
}

void get_any_key() {
	while (!kbhit())
		utility_sleep(8);
}

// Generate random number
// Number range: 0..2147483647
// 난수 생성
// 범위: 0..2147483647
int utility_random_state = 0; // 난수생성기의 상태 저장
int utility_random() {
	utility_random_state = (1103515245 * utility_random_state + 12345) & 0x7FFFFFFF;
	return utility_random_state;
}
void random_set_seed(int seed) {
	utility_random_state = seed;
	utility_random();
}

void utility_hidecursor() {
	#ifdef _WIN32 // 윈도일 때
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
	#else
	printf("\e[?25l");
	#endif
}

#endif
