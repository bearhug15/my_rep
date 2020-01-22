#include "random_gamer.h"
#include <iostream>
#include<time.h>
random_gamer::random_gamer()
{
}

 std::string random_gamer::get_id()
{
	return id;
}

cell_stat random_gamer::shoot(field* enemy_field)
{
	unsigned int width = enemy_field->get_width();
	unsigned int height = enemy_field->get_height();
	cell_stat stat;
	unsigned x = std::rand() % (width) + 1;
	unsigned y = std::rand() % (height) + 1;
	/*while (enemy_field->get_cell_stat(x, y) != cell_stat::empty_cell) { 
		x++;
		if (x > width) {
			x = 1;
			y++;
		}
		if (y > height) {
			y = 1;
		}
	}*/
	stat = enemy_field->shoot(x, y);
	
	return stat;
}

void random_gamer::set_boats(field* my_field)
{
	place_boat(4, my_field);
	//std::cout << "placed battleship"<<std::endl;
	place_boat(3, my_field);
	//std::cout << "placed cruiser" << std::endl;
	place_boat(2, my_field);
	//std::cout << "placed destroyer" << std::endl;
	place_boat(1, my_field);
	//std::cout << "placed t_boat" << std::endl;
}

void random_gamer::clear()
{
}
void random_gamer::place_boat(int size, field* my_field) {
	direction dir = direction::no_dir;
	unsigned int width = my_field->get_width();
	unsigned int height = my_field->get_height();
	for (int i = 0; i < 5 - size; i++) {
		unsigned int x = 0, y = 0;
		while (true) {
			dir = static_cast<direction>((unsigned char)(std::rand() % 4));
			x = std::rand() % (width) + 1;
			y = std::rand() % (height) + 1;	
			try {
				my_field->set_boat(x, y, size, dir);
				break;
			}
			catch (emplacement_error) {}
		}
	}
}