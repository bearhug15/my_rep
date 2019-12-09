#pragma once
#include <string>
#include <vector>
#include <iterator>
#include<fstream>
#include"validator.h"
class parser {
	std::ifstream input;
	std::string data;
	size_t processed_data=0;
public:
	parser();
	parser(std::string filename);
	~parser();
	bool end_of_instr();
	void start_of_instr();
	void parse_new_instruction_line();
	long long get_num();
	std::vector<std::string> get_params(validator*);
	size_t get_next_con_block_num();
	parser& operator++();
	std::string get_conveyor();
};
bool is_comand(std::string);