#pragma once
#include<vector>
#include"cell_stat.h"
#include"boat_stat.h"
class boat {
protected:
	bool placed = false;
	std::vector<cell_stat*> boat_coord;
public:
	virtual void set_coordinates(std::vector<cell_stat*> coord) = 0;
	virtual boat_stat check_stat() = 0;
	virtual bool is_placed() = 0;
};
