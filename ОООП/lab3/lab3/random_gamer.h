#pragma once
#include "strategy.h"
#include "my_exceptions.h"
class random_gamer : public strategy {
	 std::string id = "random_gamer";
public:
	random_gamer();
	virtual  std::string get_id();
	virtual cell_stat shoot(field* enemy_field);
	virtual void set_boats(field* my_field);
	void clear();
	void place_boat(int size, field* my_field);
};