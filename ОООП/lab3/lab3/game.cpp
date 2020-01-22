#include "game.h"

game::game(strategy* f_strat, strategy* s_strat, int rounds)
{	
	first_player_turn = true;
	first_player_win = 0;
	second_player_win = 0;
	rounds_left = rounds;
	cur_player = new player(f_strat);
	enemy_player = new player(s_strat);
	
}

void game::show_enemy_field()
{
	enemy_player->show_field_to_enemy();
}

std::pair<int, int> game::game_stat()
{
	std::pair<int, int> p(first_player_win,second_player_win);
	return p;
}
void game::start()
{	
	for (int r = rounds_left; r > 0; r--) {
		long long turn = 0;
		std::cout << "First player setting boats."<<std::endl;
		cur_player->set_boats();
		std::cout << "Second player setting boats."<<std::endl;
		enemy_player->set_boats();
		try {
			
			while (true) {
				while (true) {
					if (cur_player->get_id() == "console_gamer") {
						std::cout << "choose option:(shoot,next turn, show field, stats)" << std::endl;
						std::string str1, str2;
						std::cin >> str1 >> str2;
						if (str1 == "shoot") {
							cell_stat res = cur_player->shoot(enemy_player->get_field_p());
							if (res == cell_stat::damaged_cell) {
								std::cout << "Success!" << std::endl;
								bool game_con = enemy_player->check_boats();
								if (!game_con) {
									if (first_player_turn) { first_player_win++; throw(round_end("first player")); }
									else {
										second_player_win++; throw(round_end("second player"));
									}

								}
							}
							else {
								std::cout << "Miss :(" << std::endl;
							}

							break;
						}
						if (str1 == "skip" && str2 == "turn") {
							break;
						}
						if (str1 == "show" && str2 == "field") {
							show_enemy_field();
						}
						if (str1 == "end") {
							throw(end());
						}
					}
					else {
						cell_stat res = cur_player->shoot(enemy_player->get_field_p());
						if (res == cell_stat::damaged_cell) {	
							//std::cout << "damage" << std::endl;
							bool game_con = enemy_player->check_boats();
							if (!game_con) {
								if (first_player_turn) { first_player_win++; throw(round_end("first player")); }
								else {
									second_player_win++; throw(round_end("second player"));
								}
							}
							
						}
						else {
							break;
						}
						
					}
				}
				//std::cout << "shoot "<<turn << std::endl;
				player* baff = cur_player;
				cur_player = enemy_player;
				enemy_player = baff;
				first_player_turn = !first_player_turn;
				turn++;
			}
			
		}
		catch (round_end& e) {
			std::cout << e.so_what() << std::endl;
		}
		std::cout << "Total turns: "<<turn << std::endl;

		cur_player->clear();
		enemy_player->clear();
		if (!first_player_turn) {
			player* b_player = cur_player;
			cur_player = enemy_player;
			enemy_player = b_player;
			first_player_turn = true;
		}

	}
	throw(end());
}


