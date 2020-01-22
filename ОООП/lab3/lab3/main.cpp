#include<iostream>
#include<fstream>
#include<cstdlib>
#include"optionparser.h"
#include"game.h"
#include <time.h>
enum  optionIndex { UNKNOWN, HELP, FIRST, SECOND, COUNT };


const option::Descriptor usage[]{
	{HELP,0,"h","help",option::Arg::None,"get help"},
	{FIRST,0,"f","first",option::Arg::Optional,"get first player type"},
	{SECOND,0,"s","second",option::Arg::Optional,"get second player type"},
	{COUNT,0,"c","count",option::Arg::Optional,"get number of games"}
};


int main(int argc, char** argv) {
	srand(time(NULL));
	argc -= (argc > 0); argv += (argc > 0);
	option::Stats  stats(usage, argc, argv);
	std::vector<option::Option> options(stats.options_max);
	std::vector<option::Option> buffer(stats.buffer_max);
	option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);
	if (parse.error()) {
		return 1;
	}
	
	if (options[HELP]) {
		std::cout	<<"Tree kind of players: console_gamer, random_gamer, clever_gamer. \n" 
					<<"For console_gamer for comands: skip turn, shoot, show field. \n "
					<<"To end game: end\n";
		return 0;
	}
	game* new_game = nullptr;
	strategy* f_strat = nullptr, * s_strat = nullptr;
	int count = 1;
	try {
		
		switch (options[FIRST].count())
		{
		case 0:
			f_strat = new random_gamer(); break;
		case 1:
			if (options[FIRST].arg == NULL){break;}
			if (std::string(options[FIRST].arg) == "random_gamer") { f_strat = new random_gamer(); break; }
			if (std::string(options[FIRST].arg) == "console_gamer") { f_strat = new console_gamer(); break; }
			if (std::string(options[FIRST].arg) == "clever_gamer") { f_strat = new clever_gamer(); break; }
			throw(parameters_error("Wrong parameters1"));
		default:
			throw(parameters_error("To many parameters"));
		}
		switch (options[SECOND].count())
		{
		case 0:
			s_strat = new random_gamer(); break;
		case 1:
			if (options[SECOND].arg == NULL) { break; }
			if (std::string(options[SECOND].arg) == "random_gamer") { s_strat = new random_gamer(); break; }
			if (std::string(options[SECOND].arg) == "console_gamer") { s_strat = new console_gamer(); break; }
			if (std::string(options[SECOND].arg) == "clever_gamer") { s_strat = new clever_gamer(); break; }
			throw(parameters_error("Wrong parameters2"));
		default:
			throw(parameters_error("To many parameters"));
		}
		switch (options[COUNT].count())
		{
		case 0:
			count = 1; break;
		case 1:
			if (options[SECOND].arg == NULL) { break; }
			count = atoi(options[COUNT].arg); break;
		default:
			throw(parameters_error("To many parameters"));
		}
		/*
		delete f_strat;
		f_strat = new random_gamer();
		delete s_strat;
		s_strat = new clever_gamer();
		count = 5;*/
		new_game = new game(f_strat, s_strat, count);
		new_game->start();
	}
	catch (parameters_error& e) {
		std::cout << e.so_what();
		return 0;
	}
	catch (end & e) {
		std::cout << e.so_what();
		if (new_game == nullptr) {
			std::cout << "Something turned bad";
			return 0;
		}
		std::pair<int, int> game_stat = new_game->game_stat();
		std::cout <<std::endl <<"First player won " << game_stat.first << " games" << "Second player won " << game_stat.second << " games";
		delete f_strat;
		delete s_strat;
		return 0;
	}


	return 0;
}
