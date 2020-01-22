#pragma once
#include"player.h"
class game {
	int rounds_left;
	// player *f_player, *s_player;
	player *cur_player, *enemy_player;
	unsigned int first_player_win, second_player_win;
	bool first_player_turn;
	//field *cur_player_field, *enemy_player_field;
public:
	game(strategy* f_strat, strategy* s_strat, int rounds);
	void show_enemy_field();
	void start();
	std::pair<int,int> game_stat();
};