#pragma once
#include"strategy.h"
#include"my_exceptions.h"
class clever_gamer : public strategy {
	 std::string id = "clever_gamer";
	std::pair<unsigned int, unsigned int> last_hit;
	std::pair<unsigned int, unsigned int> last_shoot;
	direction counted_dir;
	unsigned int used_strat;
	unsigned int strat_step;
	field* c_field;
public:
	void clear();
	std::string get_id();
	clever_gamer();
	~clever_gamer();
	virtual cell_stat shoot(field* enemy_field);
	void set_boats(field* my_field);
};
