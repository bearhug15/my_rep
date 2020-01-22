#include "field.h"

boat* field::create_boat(int lenth, std::vector<cell_stat*> coord)
{
	boat* new_boat = nullptr;
	switch (lenth) {
	case 1:
		new_boat = new t_boat();
		new_boat->set_coordinates(coord);
		break;
	case 2:
		new_boat = new destroyer();
		new_boat->set_coordinates(coord);
		break;
	case 3:
		new_boat = new cruiser();
		new_boat->set_coordinates(coord);
		break;
	case 4:
		new_boat = new battleship();
		new_boat->set_coordinates(coord);
		break;

	}
	return new_boat;
}

void field::set_cell(unsigned int x, unsigned int y,cell_stat stat)
{
	play_field[y][x] = stat;
}

field::field(unsigned int height, unsigned int width):height(height),width(width)
{
	cell_stat* baff = new cell_stat[(height+2) * (width+2)];
	play_field = new cell_stat*[height+2];
	for (unsigned int i = 0; i < height+2; i++) {
		play_field[i] = baff+i * (width+2);
	}
	for (unsigned int i = 0; i < height + 2; i++) {
		for (unsigned int j = 0; j < width + 2; j++) {
			play_field[i][j] = cell_stat::empty_cell;
		}
	}
}

field::~field()
{
	delete[] play_field[0];
	delete[] play_field;
	play_field = nullptr;
	for (auto& p : boats) {
		for (auto& b : p.second) {
			delete b;
			b = nullptr;
		}
	}
}
cell_stat field::shoot(unsigned int x, unsigned int y)
{
	
	if (x<1 || x>width || y<1 || y>height) {
		throw(shooting_out_of_field(",try again."));
	}
	if (play_field[y][x] == cell_stat::boat_cell || play_field[y][x] == cell_stat::damaged_cell) {

		play_field[y][x] = cell_stat::damaged_cell;

		return cell_stat::damaged_cell;
	}
	else {
		play_field[y][x] = cell_stat::miss_cell;
		return cell_stat::miss_cell;
	}
	
	//return play_field[y][x];
}

cell_stat field::get_cell_stat(unsigned int x, unsigned int y)
{
	if (play_field[y][x] == cell_stat::boat_cell) {
		return cell_stat::empty_cell;
	}
	return play_field[y][x];
}

void field::set_boat(unsigned int x, unsigned  int y, unsigned  int lenth, direction dir)
{
	if (x<1 || y<1 || x>width || y>height) {
		throw(emplacement_error("out of field"));
	}
	
	switch (dir) {
	case direction::up:if (((int)y - ((int)lenth -1)) < 1) { throw(emplacement_error("boat out of field")); }break;
	case direction::down:if (((int)y + ((int)lenth -1)) > height) { throw(emplacement_error("boat out of field")); }break;
	case direction::left:if (((int)x - ((int)lenth -1 )) < 1) { throw(emplacement_error("boat out of field")); }break;
	case direction::right:if (((int)x + ((int)lenth -1)) > width) { throw(emplacement_error("boat out of field")); }break;
	}

	std::vector<cell_stat*> coord;
	int idir = static_cast<int>(dir);
	switch (dir) {
	case direction::up:
	case direction::down:
		

		for (int i = -1; i != 2; i++) {
			for (int j = -1; j != lenth+1; j++) {
				if (play_field[y + int(j * pow(-1, idir+1))][x+ i] != cell_stat::empty_cell) {
					throw(emplacement_error("place occupied"));
				}
			}
		}
		for (int j = 0; j != lenth; j++) {
			play_field[y + int(j * pow(-1, idir + 1))][x] = cell_stat::boat_cell;
			coord.push_back(play_field[y+ int(j * pow(-1, idir + 1))]+x);
		}
		break;
	case direction::left:
	case direction::right:
		for (int i = -1; i != 2; i++) {
			for (int j = -1; j != lenth+1; j++) {
				if (play_field[y + i][x + int(j * pow(-1, idir+1))] != cell_stat::empty_cell) {
					throw(emplacement_error("place occupied"));
				}
			}
		}
		for (int j = 0; j != lenth; j++) {
			play_field[y][x + int(j * pow(-1, idir + 1))] = cell_stat::boat_cell;
			coord.push_back(play_field[y] + x+int(j * pow(-1, idir + 1)));
		}
		break;
	}
	boat *new_boat = create_boat(lenth, coord);
	boats[lenth+1].push_back(new_boat);
}




unsigned int field::get_height()
{
	return height;
}
unsigned int field::get_width()
{
	return width;
}

bool field::check_boats()
{
	boat_stat stat = boat_stat::destroyed;
	for (auto& boat_t : boats) {
		for (auto& boat : boat_t.second) {
			if (boat->check_stat() == boat_stat::normal) { stat = boat_stat::normal; }
		}
	}
	if(stat== boat_stat::normal){
		return true;
	}
	return false;
}
