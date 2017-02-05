#include <stdio.h>
#include <time.h>

#include "update.h"
#include "display.h"
#include "player.h"
#include "utility.h"

int b_game_exit = 0;
int b_difficulty_selected = 0;
int game_difficulty;

int main(int argc, char const *argv[]) {
	utility_hidecursor();
	random_set_seed((int) time(NULL)); // initialise RNG 난수생성기 초기화

start_game:

	display_wipe_screen();
	
	// Get difficulty selection
	// 난이도 선택 받기  
	b_difficulty_selected = 0;
	while (!b_difficulty_selected) {
		b_difficulty_selected = update_select_difficulty();
		display_floor();
		display_select_difficulty(difficulty_selection);
	}
	game_difficulty = difficulty_selection;

	// Initialise the whole game
	// 전체 게임을 초기화함
	Player player = player_new();
	Block block = block_new();
	Stage stage = stage_new(game_difficulty); // stage_new(난이도)
	
	display_wipe_screen();
	display_frame();
	
	// main game loop
	while (!b_game_exit) {
		update_all(&player, &block, &stage);
		display_all(&player, &block, &stage);

		// GAME OVER
		if (player.lives == 0) {
			display_game_over(&player, &stage);
			 
			utility_sleep(2000);
			get_any_key();
			
			utility_sleep(100); // prevent input from over-getting
			goto start_game;
		}
	}
	
}


