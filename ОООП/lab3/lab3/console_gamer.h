#pragma once
#include"strategy.h"
#include"optionparser.h"
#include"my_exceptions.h"
#include<iostream>
class console_gamer : public strategy {
	 std::string id = "console_gamer";
	void place_boat(int size, field* my_field);
public:
	 std::string get_id();
	cell_stat shoot(field* enemy_field);
	void set_boats(field* my_field);
	void clear();
};