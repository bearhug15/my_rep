#pragma once
#include"cell_stat.h"
#include"boats.h"
#include"my_exceptions.h"
#include<map>
#include<vector>
#include<string>
#include "direction.h"
class field {
	cell_stat** play_field;
	unsigned int height = 0;
	unsigned int width = 0;
	std::map<int, std::vector<boat*>> boats;
	boat* create_boat(int lenth, std::vector<cell_stat*> coord);
	void set_cell(unsigned int x, unsigned int y, cell_stat stat);
	friend  class clever_gamer;
public:
	field(unsigned int height, unsigned int width);
	~field();
	cell_stat shoot(unsigned int x, unsigned int y);
	cell_stat get_cell_stat(unsigned int x, unsigned int y);
	void set_boat(unsigned int x, unsigned  int y, unsigned  int lenth, direction dir);
	unsigned int get_height();
	unsigned int get_width();
	bool check_boats();
};
