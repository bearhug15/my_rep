#define filling() {switch (counted_dir) {\
					case direction::up:bi = 0; bj = -1; break;\
					case direction::down:bi = 0; bj = 1; break;\
					case direction::left:bi = -1; bj = 0; break;\
					case direction::right:bi = 1; bj = 0; break;\
					case direction::no_dir:bi = 0; bj = 0; break;\
					}\
					for (int i = -1; i < 2; i++) {\
						for (int j = -1; j < 2; j++) {\
							if (i != bi || j != bj) {\
								c_field->set_cell(last_hit.first + i, last_hit.second + j, cell_stat::miss_cell);\
							}\
						}\
					}\
					};


#include "clever_gamer.h"
#include<iostream>
#include <time.h>
void clever_gamer::clear()
{
	last_hit.first = 0;
	last_hit.second = 1;
	last_shoot.first = 0;
	last_shoot.second = 1;
	used_strat = 0;
	strat_step = 0;
	delete c_field;
	c_field = new field(10,10);
	counted_dir = direction::no_dir;
}

std::string clever_gamer::get_id()
{
	return id;
}

clever_gamer::clever_gamer()
{
	last_hit.first = 0;
	last_hit.second = 1;
	last_shoot.first = 0;
	last_shoot.second = 1;
	counted_dir = direction::no_dir;
	used_strat = 0;
	strat_step = 0;
	c_field = new field(10,10);
}

clever_gamer::~clever_gamer()
{
	delete c_field;
	c_field = nullptr;
}

cell_stat clever_gamer::shoot(field* enemy_field)
{
	int bi = -2, bj = -2;
	unsigned int width = enemy_field->get_width();
	unsigned int height = enemy_field->get_height();
	cell_stat shoot_result = cell_stat::empty_cell;
	if (last_hit.first == 0) {
shoot:
		switch (used_strat) {
		case 0:
			while (true) {
				if ((last_shoot.first + 4) < (width + 1)) {
					last_shoot.first = last_shoot.first + 4;

				}
				else {
					if (strat_step == 24) {
						strat_step = 0;
						last_shoot.first = -2;
						last_shoot.second = 1;
						used_strat++;
						goto shoot;
					}
					if (last_shoot.first != width) {
						last_shoot.first = (last_shoot.first + 5) % width;
						last_shoot.second++;
					}
					else {
						last_shoot.first = 1;
						last_shoot.second++;
					}
				}
				if (c_field->get_cell_stat(last_shoot.first, last_shoot.second) == cell_stat::empty_cell) {
					shoot_result = enemy_field->shoot(last_shoot.first, last_shoot.second);
					c_field->set_cell(last_shoot.first, last_shoot.second, shoot_result);
					if (shoot_result == cell_stat::damaged_cell) {
						last_hit.first = last_shoot.first;
						last_hit.second = last_shoot.second;
					}
					strat_step++;
					break;
				}
				strat_step++;

			}break;
		case 1:
			while (true) {
				if ((last_shoot.first + 4) < (width + 1)) {
					last_shoot.first = last_shoot.first + 4;

				}
				else {
					if (strat_step == 26) {
						strat_step = 0;
						last_shoot.first = 0;
						last_shoot.second = 1;
						used_strat++;
						goto shoot;
					}
					if (last_shoot.first != width) {
						last_shoot.first = (last_shoot.first + 5) % width;
						last_shoot.second++;
					}
					else {
						last_shoot.first = 1;
						last_shoot.second++;
					}
				}
				if (c_field->get_cell_stat(last_shoot.first, last_shoot.second) == cell_stat::empty_cell) {
					shoot_result = enemy_field->shoot(last_shoot.first, last_shoot.second);
					c_field->set_cell(last_shoot.first, last_shoot.second, shoot_result);
					if (shoot_result == cell_stat::damaged_cell) {
						last_hit.first = last_shoot.first;
						last_hit.second = last_shoot.second;
					}
					strat_step++;
					return shoot_result;
				}
				strat_step++;

			}break;
		case 2:
			int x = std::rand() % (width) + 1;
			int y = std::rand() % (height) + 1;
			int xb = x,yb=y;
			int t=0;
			while (c_field->get_cell_stat(x, y) != cell_stat::empty_cell) {
				t++;
				x++;
				if (x > width) {
					x = 1;
					y++;
				}
				if (y > height) {
					y = 1;
				}
				/*if (xb == x && yb == y) {
					std::cout << "WTF?!"<<std::endl;
					system("pause");
					return cell_stat::miss_cell;
				}
				else {
					std::cout << t << std::endl;
				}*/
			}
			last_shoot.first = x;
			last_shoot.second = y;
			shoot_result = enemy_field->shoot(last_shoot.first, last_shoot.second);
			c_field->set_cell(last_shoot.first, last_shoot.second, shoot_result);
			if (shoot_result == cell_stat::damaged_cell) {
				last_hit.first = last_shoot.first;
				last_hit.second = last_shoot.second;
			}
		}

	}
	else {
	if (used_strat == 2)goto shoot;
	if (counted_dir == direction::no_dir) {
		if ((last_hit.second > 1) && (c_field->get_cell_stat(last_hit.first, last_hit.second - 1) == cell_stat::empty_cell)) {
			shoot_result = enemy_field->shoot(last_hit.first, last_hit.second - 1);
			c_field->set_cell(last_hit.first, last_hit.second - 1, shoot_result);
			if (shoot_result == cell_stat::damaged_cell) {
				last_hit.first = last_hit.first;
				last_hit.second = last_hit.second - 1;
				counted_dir = direction::up;
				filling();
			}
			return shoot_result;
		}
		if ((last_hit.second < height) && (c_field->get_cell_stat(last_hit.first, last_hit.second + 1) == cell_stat::empty_cell)) {
			shoot_result = enemy_field->shoot(last_hit.first, last_hit.second + 1);
			c_field->set_cell(last_hit.first, last_hit.second + 1, shoot_result);
			if (shoot_result == cell_stat::damaged_cell) {
				last_hit.first = last_hit.first;
				last_hit.second = last_hit.second + 1;
				counted_dir = direction::down;
				filling();
			}
			return shoot_result;
		}
		if ((last_hit.first > 1) && (c_field->get_cell_stat(last_hit.first - 1, last_hit.second) == cell_stat::empty_cell)) {
			shoot_result = enemy_field->shoot(last_hit.first - 1, last_hit.second);
			c_field->set_cell(last_hit.first - 1, last_hit.second, shoot_result);
			if (shoot_result == cell_stat::damaged_cell) {
				last_hit.first = last_hit.first - 1;
				last_hit.second = last_hit.second;
				counted_dir = direction::left;
				filling();
			}
			return shoot_result;
		}
		if ((last_hit.first  < width ) && (c_field->get_cell_stat(last_hit.first + 1, last_hit.second) == cell_stat::empty_cell)) {
			shoot_result = enemy_field->shoot(last_hit.first + 1, last_hit.second);
			c_field->set_cell(last_hit.first + 1, last_hit.second, shoot_result);
			if (shoot_result == cell_stat::damaged_cell) {
				last_hit.first = last_hit.first + 1;
				last_hit.second = last_hit.second;
				counted_dir = direction::right;
				filling();
			}
			return shoot_result;
		}
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i != bi && j != bj) {
					c_field->set_cell(last_hit.first + i, last_hit.second + j, cell_stat::miss_cell);
				}
			}
		}
		goto shoot;
	}
	else {
		switch (counted_dir) {
		case direction::up:
			if ((last_hit.second < 2) || c_field->get_cell_stat(last_hit.first, last_hit.second - 1) != cell_stat::empty_cell){
				counted_dir = direction::no_dir;
				filling();
				goto shoot;
			} else {
				shoot_result = enemy_field->shoot(last_hit.first, last_hit.second-1);
				c_field->set_cell(last_hit.first, last_hit.second-1, shoot_result);
				if (shoot_result == cell_stat::damaged_cell) {
					last_hit.first = last_hit.first;
					last_hit.second = last_hit.second-1;
					filling();
				}
				return shoot_result;
			}
		case direction::down:
			if (last_hit.second + 1 >height || c_field->get_cell_stat(last_hit.first, last_hit.second + 1) != cell_stat::empty_cell){
				counted_dir = direction::no_dir;
				filling();
				goto shoot;
			} else {
					shoot_result = enemy_field->shoot(last_hit.first, last_hit.second + 1);
					c_field->set_cell(last_hit.first, last_hit.second + 1, shoot_result);
					if (shoot_result == cell_stat::damaged_cell) {
						last_hit.first = last_hit.first;
						last_hit.second = last_hit.second + 1;
						filling();
					}
					return shoot_result;
				}
		case direction::left:
			if (last_hit.first < 2 || c_field->get_cell_stat(last_hit.first-1, last_hit.second) != cell_stat::empty_cell){
				counted_dir = direction::no_dir;
				filling();
				goto shoot;
			} else {
					shoot_result = enemy_field->shoot(last_hit.first-1, last_hit.second );
					c_field->set_cell(last_hit.first-1, last_hit.second, shoot_result);
					if (shoot_result == cell_stat::damaged_cell) {
						last_hit.first = last_hit.first-1;
						last_hit.second = last_hit.second;
						filling();

					}
					return shoot_result;
				}
		case direction::right:
			if (last_hit.first + 1 >width || c_field->get_cell_stat(last_hit.first + 1, last_hit.second) != cell_stat::empty_cell){
				counted_dir = direction::no_dir;
				filling();
				goto shoot;
			} else {
					shoot_result = enemy_field->shoot(last_hit.first+1, last_hit.second );
					c_field->set_cell(last_hit.first + 1, last_hit.second, shoot_result);
					if (shoot_result == cell_stat::damaged_cell) {
						last_hit.first = last_hit.first + 1;
						last_hit.second = last_hit.second;
						filling();
					}
					return shoot_result;
				}
			}

		}

	}
	return shoot_result;
	
}

void clever_gamer::set_boats(field* my_field)
{
	unsigned int width = my_field->get_width();
	unsigned int height = my_field->get_height();
	int x = 1, y = 1;
	direction dir = direction::up;

	x = rand() % (width - 3) + 1;
	my_field->set_boat(x, y, 4, direction::right);
	
	for (int i = 0; i < 2; i++) {
		x = 1 + i*(width-1);
		while (true) {
			y = rand() % (height - 2) + 1;
			try {
				my_field->set_boat(x, y, 3, direction::down);
				break;
			}
			catch (emplacement_error& e) {}
		}
		while (true) {
			y = rand() % (height - 1) + 1;
			try {
				my_field->set_boat(x, y, 2, direction::down);
				break;
			}
			catch (emplacement_error& e) {}
		}
	}

	y = height;
	while (true) {
		try {
			x = rand() % (width - 1) + 1;
			my_field->set_boat(x, y, 2, direction::right);
			break;
		}
		catch (emplacement_error& e) {}
	}
	for (int i = 0; i < 4; i++) {
		while (true) {
			x = rand() % (width)+1;
			y = rand() % (height)+1;
			try {
				my_field->set_boat(x,y,1,direction::up);
				break;
			}
			catch(emplacement_error& e){}
		}
	}
	//std::cout << "Boats placed" << std::endl;
	/*
	while (true) {
		int idir = std::rand() % 4;
		dir = static_cast<direction>(idir);
		idir = idir % 2;
		try {
			switch (dir) {
			case direction::up:
			case direction::down:
				y = y + idir * (height - 1);
				x = x + std::rand() % (width - 1);
				my_field->set_boat(x, y, 4, direction::right);
				break;
			case direction::left:
			case direction::right:
				x = x + (idir%2) * (width - 1);
				y = y + std::rand() % (height - 1);
				my_field->set_boat(x, y, 4, direction::down);
				break;
			}

			break;
		}
		catch (emplacement_error) {}
	}
	std::cout << " set battleship" << std::endl;
	for (int i = 0; i < 2; i++) {
		while (true) {
			int idir = std::rand() % 4;
			dir = static_cast<direction>(idir);
			idir = idir % 2;
			try {
				switch (dir) {
				case direction::up:
				case direction::down:
					y = y + idir * (height - 1);
					x = x + std::rand() % (width - 1);
					my_field->set_boat(x, y, 3, direction::right);
					break;
				case direction::left:
				case direction::right:
					x = x + (idir % 2) * (width - 1);
					y = y + std::rand() % (height - 1);
					my_field->set_boat(x, y, 3, direction::down);
					break;
				}

				break;
			}
			catch (emplacement_error) {}
		}
	}
	std::cout << " set destroyer" << std::endl;
	for (int i = 0; i < 3; i++) {
		while (true) {
			int idir = std::rand() % 4;
			dir = static_cast<direction>(idir);
			idir = idir % 2;
			try {
				switch (dir) {
				case direction::up:
				case direction::down:
					y = y + idir * (height - 1);
					x = x + std::rand() % (width - 1);
					my_field->set_boat(x, y, 2, direction::right);
					break;
				case direction::left:
				case direction::right:
					x = x + (idir % 2) * (width - 1);
					y = y + std::rand() % (height - 1);
					my_field->set_boat(x, y, 2, direction::down);
					break;
				}

				break;
			}
			catch (emplacement_error) {}
		}
	}
	std::cout << " set cruiser" << std::endl;
	for (int i = 0; i < 4; i++) {
		while (true) {
			x = std::rand() % (width - 1) + 1;
			y = std::rand() % (height - 1) + 1;
			try {
				my_field->set_boat(x, y, 1, direction::up);
				break;
			}
			catch (emplacement_error) {}
		}
	}
	std::cout << "set t_boat" << std::endl;*/
}
