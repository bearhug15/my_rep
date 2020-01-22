#include "console_gamer.h"

 std::string console_gamer::get_id()
{
	return id;
}

cell_stat console_gamer::shoot(field* enemy_field)
{
	int x = 0, y = 0; 
	cell_stat shoot_result;
	while (true) {
		std::cout << "Choose coordinates for shooting:";
		std::cin >> x >> y;
		
		try {
			shoot_result = enemy_field->shoot(x, y);
			break;
		}
		catch (shooting_out_of_field & e) {
			std::cout << e.what();

		}
	}
	return shoot_result;
}




void console_gamer::place_boat(int size, field* my_field) {
	std::map<std::string, direction> direction_map;
	direction_map["up"] = direction::up;
	direction_map["down"] = direction::down;
	direction_map["left"] = direction::left;
	direction_map["right"] = direction::right;
	direction dir = direction::no_dir;
	std::string str ="up";
	for (int i = 0; i < 5-size; i++) {
		int x = 0, y = 0;
		if (size != 1) {
			std::cout << "Set x,y,direction for boat of size "<<size<<"\n";
			std::cin >> x >> y >> str;
		}
		else {
			std::cout << "Set x,y for boat of size " << size << "\n";
			std::cin >> x >> y;
		}
		if (direction_map.find(str) != direction_map.end()) {
			dir = direction_map[str];
			try {
				my_field->set_boat(x, y, size, dir);
			}
			catch (emplacement_error) {
				std::cout << "Can`t emplace, try again.\n";
				i--;
			}
		}
		else {
			std::cout << "Direction not exist, try again.";
			i--;
		}

	}
}

void console_gamer::set_boats(field* my_field)
{
	place_boat(1, my_field);
	place_boat(2, my_field);
	place_boat(3, my_field);
	place_boat(4, my_field);
	
}

void console_gamer::clear()
{
}

