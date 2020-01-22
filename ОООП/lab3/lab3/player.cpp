#include "player.h"

player::player(strategy* my_s)
{
	my_strat = my_s;
	my_field = new field(10, 10);
}

player::~player()
{
	delete my_field;
	my_field = nullptr;
}

void player::set_boats()
{
	my_strat->set_boats(my_field);
}

cell_stat player::shoot(field* enemy_field)
{
	return my_strat->shoot(enemy_field);
}

field* player::get_field_p()
{
	return my_field;
}

bool player::check_boats()
{
	return my_field->check_boats();
}

 std::string player::get_id()
{
	return my_strat->get_id();
}

void player::show_field_to_enemy()
{
	for (unsigned int i = 0; i < my_field->get_height(); i++) {
		for (unsigned int j = 0; j < my_field->get_width(); j++) {
			cell_stat stat = my_field->get_cell_stat(j, i);
			switch (stat) {
			case cell_stat::damaged_cell:std::cout << "*"; break;
			case cell_stat::miss_cell: std::cout << " "; break;
			case cell_stat::empty_cell:
			case cell_stat::boat_cell:std::cout << "~";
			}
		}
		std::cout << std::endl;
	}
}

void player::clear()
{
	field* baff_field = new field(my_field->get_height(), my_field->get_width());
	delete my_field;
	my_field = baff_field;
	my_strat->clear();
}
