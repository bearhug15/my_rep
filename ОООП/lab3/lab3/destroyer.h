#pragma once
#include "boat.h"
class destroyer : public boat {
public:
	destroyer() {
		placed = false;
	}
	virtual void set_coordinates(std::vector<cell_stat*> coord) {
		placed = true;
		boat_coord = coord;
	}
	virtual boat_stat check_stat() {
		for (unsigned int i = 0; i < boat_coord.size(); i++) {
			if (*(boat_coord[i]) == cell_stat::boat_cell) {
				return boat_stat::normal;
			}
		}
		return boat_stat::destroyed;
	}
	virtual bool is_placed() {
		return placed;
	}
};