#pragma once
#include"cell_stat.h"
#include "field.h"
#include<unordered_map>
#include<vector>
#include"boats.h"
class strategy {
public:
	virtual cell_stat shoot( field* enemy_field) = 0;
	virtual void set_boats(field* my_field) = 0;
	virtual  std::string get_id() = 0;
	void virtual clear()=0;
};