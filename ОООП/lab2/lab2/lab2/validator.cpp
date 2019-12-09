#include"validator.h"
#include "my_exceptions.h"
#include <sstream>

validator::validator() {
	comand_map = nullptr;
}

template <typename T>
std::string to_string(T val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

void validator::validate_con(std::string* data, std::unordered_map<size_t, std::vector<std::string>>* params_map) {
	size_t processed_data = 0;
	while (true) {
		size_t baff_pos = data->find_first_not_of(" ", processed_data);
		if (baff_pos == std::string::npos)return;
		if ((data[0][baff_pos] != '-' || data[0][baff_pos + 1] != '>') && processed_data != 0)throw (conveyor_docking_error("wrong conveyor construction"));
		if (data[0][baff_pos] == '-')baff_pos += 2;
		processed_data = data->find_first_not_of(" ", baff_pos);
		baff_pos = data->find_first_not_of("123456789", processed_data);
		if (processed_data == baff_pos) throw(conveyor_docking_error("wrong conveyor construction"));
		std::string num_data = data->substr(processed_data, baff_pos != std::string::npos ? baff_pos - processed_data : std::string::npos);
		processed_data = baff_pos;
		size_t num = stoll(num_data);
		if (params_map->find(num) == params_map->end())throw(conveyor_docking_error(std::string("comand with number ") + to_string(num) + std::string(" not exists")));
	}
}

void validator::validate_comand(std::string * com) {
	if (comand_map == nullptr)throw(std::exception(""));
	if (comand_map->find(com[0]) == comand_map->end()) throw instruction_list_error("wrong comand name");
}

void validator::set_comand_map(std::unordered_map<std::string, worker*>* comand_map) {
	this->comand_map = comand_map;
	return;
}