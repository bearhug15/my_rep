#pragma once
#include"field.h"
#include"strategy.h"
#include<unordered_map>
#include<vector>
#include<iostream>
#include"strategies.h"

class player {
	field* my_field;
	strategy* my_strat;
	
public:
	player(strategy* my_s);
	~player();
	void set_boats();
	cell_stat shoot(field* enemy_field);
	field* get_field_p();
	bool check_boats();
	 std::string get_id();
	void show_field_to_enemy();
	void clear();
};